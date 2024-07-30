// Copyright 2024 ChickenGames All rights reserved.


#include "SoulAbilitySystemComponent.h"

#include "AbilitySystemLog.h"
#include "Soul.h"
#include "SoulGameplayTags.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Character/SoulPlayerCharacter.h"


bool USoulAbilitySystemComponent::TryBufferAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivatePtrs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivatePtrs);
	if (AbilitiesToActivatePtrs.Num() < 1)
	{
		return false;
	}

	TArray<FGameplayAbilitySpec> AbilitiesToActivate;
	AbilitiesToActivate.Reserve(AbilitiesToActivatePtrs.Num());
	Algo::Transform(AbilitiesToActivatePtrs, AbilitiesToActivate, [](FGameplayAbilitySpec* SpecPtr) { return *SpecPtr; });

	bool bSuccess = false;
	for (const FGameplayAbilitySpec& GameplayAbilitySpec : AbilitiesToActivate)
	{
		ensure(IsValid(GameplayAbilitySpec.Ability));
		bSuccess |= TryBufferAbility(GameplayAbilitySpec.Handle, bAllowRemoteActivation);
	}

	return bSuccess;
}

bool USoulAbilitySystemComponent::TryBufferAbilityByClass(TSubclassOf<UGameplayAbility> InAbilityToActivate, bool bAllowRemoteActivation)
{
	bool bSuccess = false;

	const UGameplayAbility* const InAbilityCDO = InAbilityToActivate.GetDefaultObject();
	for (const FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability == InAbilityCDO)
		{
			bSuccess |= TryBufferAbility(Spec.Handle, bAllowRemoteActivation);
			break;
		}
	}

	return bSuccess;
}

bool USoulAbilitySystemComponent::TryBufferAbility(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation)
{
	FGameplayTagContainer FailureTags;
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilityToActivate);
	if (!Spec)
	{
		ABILITY_LOG(Warning, TEXT("TryActivateAbility called with invalid Handle"));
		return false;
	}
	if (Spec->PendingRemove || Spec->RemoveAfterActivation)
	{
		return false;
	}

	UGameplayAbility* Ability = Spec->Ability;
	if (!Ability)
	{
		ABILITY_LOG(Warning, TEXT("TryActivateAbility called with invalid Ability"));
		return false;
	}

	const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	if (ActorInfo == nullptr || !ActorInfo->OwnerActor.IsValid() || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ENetRole NetMode = ActorInfo->AvatarActor->GetLocalRole();
	if (NetMode == ROLE_SimulatedProxy)
	{
		return false;
	}

	BufferedAbilityData = FBufferedAbilityData(AbilityToActivate, bAllowRemoteActivation);
	// SOUL_LOG(Display, TEXT("Buffer Added: %s"), *Spec->GetDebugString());
	return true;
}

void USoulAbilitySystemComponent::ActivateAbilityBuffer()
{
	if (BufferedAbilityData.IsValid())
	{
		TryActivateAbility(BufferedAbilityData.AbilitySpecHandle, BufferedAbilityData.bAllowRemoteActivation);
	}
	BufferedAbilityData = FBufferedAbilityData();
}

void USoulAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	SoulAvatarCharacter = Cast<ASoulPlayerCharacter>(InAvatarActor);
}

void USoulAbilitySystemComponent::OnTagUpdated(const FGameplayTag& Tag, bool TagExists)
{
	Super::OnTagUpdated(Tag, TagExists);
	if (!SoulAvatarCharacter) return;

	// Buffer 끝났을 때 현재 버퍼들을 실행
	if (Tag.MatchesTag(SoulGameplayTags::Status_Attack_Buffering) && !TagExists)
	{
		if (BufferedAbilityData.IsValid())
		{
			ActivateAbilityBuffer();
		}
		return;
	}
}

void USoulAbilitySystemComponent::AbilityLocalInputTriggered(UInputAction* InputAction)
{
	if (TriggeredActions.Contains(InputAction)) return;
	TriggeredActions.Add(InputAction);
	ABILITYLIST_SCOPE_LOCK();

	// 한번의 키에 하나의 액션 발동
	// 우선순위에 의한 어빌리티 작동을 위함.
	bool bIsActivated = false;
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (USoulGameplayAbility* SoulAbility = Cast<USoulGameplayAbility>(Spec.Ability))
		{
			if (SoulAbility->GetInputAction() == InputAction)
			{
				SOUL_LOG(Display, TEXT("%s Press!"), *InputAction->ActionDescription.ToString());

				Spec.InputPressed = true;
				AbilitySpecInputPressed(Spec);
				if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
				{
					ServerSetInputPressed(Spec.Handle);
				}
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());


				if (HasMatchingGameplayTag(SoulGameplayTags::Status_Attack_Buffering))
				{
					TryBufferAbility(Spec.Handle);
				}
				else
				{
					if (!bIsActivated)
					{
						bIsActivated = TryActivateAbility(Spec.Handle);
					}
				}
			}
		}
	}
}


void USoulAbilitySystemComponent::AbilityLocalInputCompleted(UInputAction* InputAction)
{
	if (TriggeredActions.Contains(InputAction))
		TriggeredActions.Remove(InputAction);
	
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (USoulGameplayAbility* SoulAbility = Cast<USoulGameplayAbility>(Spec.Ability))
		{
			if (SoulAbility->GetInputAction() == InputAction)
			{
				if (Spec.Ability && Spec.IsActive())
				{
					AbilitySpecInputReleased(Spec);
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputReleased(Spec.Handle);
					}


					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
				}
			}
		}
	}
}

void USoulAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Spec.InputPressed = true;
	if (Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Spec.Ability->InputPressed(Spec.Handle, AbilityActorInfo.Get(), Spec.ActivationInfo);
	}
	else
	{
		TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
		for (UGameplayAbility* Instance : Instances)
		{
			Instance->InputPressed(Spec.Handle, AbilityActorInfo.Get(), Spec.ActivationInfo);
		}
	}
}

void USoulAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Spec.InputPressed = false;
	if (Spec.Ability->GetInstancingPolicy() == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		Spec.Ability->InputReleased(Spec.Handle, AbilityActorInfo.Get(), Spec.ActivationInfo);
	}
	else
	{
		TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
		for (UGameplayAbility* Instance : Instances)
		{
			Instance->InputReleased(Spec.Handle, AbilityActorInfo.Get(), Spec.ActivationInfo);
		}
	}
}

FGameplayAbilitySpec USoulAbilitySystemComponent::BuildSoulAbilitySpecFromClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, UInputAction* InputAction)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level);
	// TODO: 이렇게 진행 시 AbilitySpec.Ability가 CDO. 현 작동은 문제가 없으나 문제 발생시나 이 방식을 뛰어 넘는 방법 고안시 수정. 
	if (USoulGameplayAbility* SoulAbility = Cast<USoulGameplayAbility>(AbilitySpec.Ability))
	{
		SoulAbility->SetInputAction(InputAction);
	}
	return AbilitySpec;
}
