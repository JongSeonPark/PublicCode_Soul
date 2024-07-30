// Copyright 2024 ChickenGames All rights reserved.


#include "SoulComboAbility.h"

#include "AbilitySystemComponent.h"
#include "Soul.h"
#include "SoulGameplayTags.h"

USoulComboAbility::USoulComboAbility()
{
	ActivationOwnedTags.AddTag(SoulGameplayTags::Combo);
}

bool USoulComboAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                           const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (ComboAbilityClasses.IsEmpty() || !Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	if (IsInstantiated())
	{
		const int32 ComboCount = GetCurrentAbilitySpec()->ActiveCount;
		const TSubclassOf<USoulGameplayAbility> CurrentComboAbility = ComboAbilityClasses[ComboCount % ComboAbilityClasses.Num()];
		const bool bSuccess = CurrentComboAbility->GetDefaultObject<USoulGameplayAbility>()->CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
		// SOUL_LOG(Display, TEXT("%s Scuess %d"), *CurrentComboAbility->GetName(), bSuccess);
		return bSuccess;
	}
	// 없으면 최초 실행
	else
	{
		const bool bSuccess = ComboAbilityClasses[0].GetDefaultObject()->CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
		// SOUL_LOG(Display, TEXT("%s Scuess %d"), *ComboAbilityClasses[0]->GetName(), bSuccess);
		return bSuccess;
	}
	// 최소 첫번째 어빌리티는 실행가능해야함.
}

void USoulComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
	if (!HasAuthorityOrPredictionKey(OwnerInfo, &ActivationInfo) || !CommitAbility(Handle, OwnerInfo, ActivationInfo)) return;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	const int32 ComboCount = GetCurrentAbilitySpec()->ActiveCount - 1;
	const TSubclassOf<USoulGameplayAbility> CurrentComboAbility = ComboAbilityClasses[ComboCount % ComboAbilityClasses.Num()];

	TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();
	for (FGameplayAbilitySpec ActivatableAbility : ActivatableAbilities)
	{
		if (ActivatableAbility.Ability.GetClass() == CurrentComboAbility)
		{
			// SOUL_LOG(Display, TEXT("Combo: %s, %d"), *ActivatableAbility.Ability.GetName(), ActivatableAbility.ActiveCount);
			ComboAbilitySpec = ActivatableAbility;
			break;
		}
	}

	// 실행 불가시 바로 어빌리티 종료
	if (!ComboAbilitySpec.Handle.IsValid() || !ASC->TryActivateAbilityByClass(CurrentComboAbility))
	{
		K2_EndAbility();
		return;
	}

	ASC->AbilityActivatedCallbacks.AddUObject(this, &USoulComboAbility::OnASCAbilityActivated);
	ASC->OnAbilityEnded.AddUObject(this, &USoulComboAbility::OnASCAbilityEnded);
}

void USoulComboAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                                   bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USoulComboAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ComboAbilitySpec.Handle.IsValid())
	{
		TArray<UGameplayAbility*> Instances = ComboAbilitySpec.GetAbilityInstances();
		for (UGameplayAbility* Instance : Instances)
		{
			Instance->InputPressed(ComboAbilitySpec.Handle, ActorInfo, ActivationInfo);
		}
	}
}

void USoulComboAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ComboAbilitySpec.Handle.IsValid())
	{
		TArray<UGameplayAbility*> Instances = ComboAbilitySpec.GetAbilityInstances();
		for (UGameplayAbility* Instance : Instances)
		{
			Instance->InputReleased(ComboAbilitySpec.Handle, ActorInfo, ActivationInfo);
		}
	}
}

TArray<TSubclassOf<USoulGameplayAbility>> USoulComboAbility::GetChildAbilities() const
{
	TArray<TSubclassOf<USoulGameplayAbility>> Results;
	for (TSubclassOf<USoulGameplayAbility> BufferAbilityClass : ComboAbilityClasses)
	{
		Results.Add(BufferAbilityClass);
		Results.Append(BufferAbilityClass->GetDefaultObject<USoulGameplayAbility>()->GetChildAbilities());
	}
	return Results;
}

void USoulComboAbility::OnASCAbilityActivated(UGameplayAbility* Ability)
{
	if (Ability->IsA<USoulComboAbility>())
	{
		SOUL_LOG(Display, TEXT("Static Equal!!!!"));
		// GetClass는 상속받은 블루프린트 클래스를 반환함.
		if (Ability->GetClass() == GetClass())
		{
			SOUL_LOG(Display, TEXT("EQUAL!!!!"));
			// 초기화 시킴으로 OnASCAbilityEnded에서 종료되지 않음.
			ComboAbilitySpec = FGameplayAbilitySpec();
		}
		// 다른 콤보가 들어왔을 때 이 어빌리티를 종료함.
		else
		{
			K2_EndAbility();
		}
	}

	SOUL_LOG(Display, TEXT("OnASCAbilityActivated, %s"), *Ability->GetName());
}

void USoulComboAbility::OnASCAbilityEnded(const FAbilityEndedData& EndedData)
{
	SOUL_LOG(Display, TEXT("OnASCAbilityEnded, %s"), *EndedData.AbilityThatEnded->GetName());

	// 일괄적으로 콤보들을 삭제하기 위해서 현재 콤보의 어빌리티가 종료되었을 때 콤보를 끝낸다.
	if (EndedData.AbilitySpecHandle == ComboAbilitySpec.Handle)
	{
		K2_EndAbility();
	}

	// 다른 콤보 어빌리티가 종료되었을 때 일괄 종료.
	if (EndedData.AbilitySpecHandle == GetCurrentAbilitySpecHandle())
	{
		K2_EndAbility();
	}
}
