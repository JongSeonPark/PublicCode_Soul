// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayAbility_ChargeAttack.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Soul.h"
#include "SoulGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"


class UEnhancedInputLocalPlayerSubsystem;

USoulGameplayAbility_ChargeAttack::USoulGameplayAbility_ChargeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void USoulGameplayAbility_ChargeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ActiveAbilityClass = ChargingAbilityClass;
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilityByClass(ChargingAbilityClass);

	ChargingEventDelegate = GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(SoulGameplayTags::Status_Attack_Charging).AddUObject(
		this, &USoulGameplayAbility_ChargeAttack::OnChargeTagChanged);

	// 버퍼로 실행되었을 때, 이 키가 눌러지고 있는지에 대한 확인.
	USoulGameplayAbility* CDO = Cast<USoulGameplayAbility>(GetCurrentAbilitySpec()->Ability);
	const TObjectPtr<UInputAction> CheckInputAction = CDO->GetInputAction() ? CDO->GetInputAction() : CDO->GetParentInputAction();
	if (CheckInputAction)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(ActorInfo->PlayerController->GetLocalPlayer()))
		{
			if (const FInputActionInstance* ActionData = Subsystem->GetPlayerInput()->FindActionInstanceData(CheckInputAction))
			{
				switch (ActionData->GetTriggerEvent())
				{
				case ETriggerEvent::Triggered:
				case ETriggerEvent::Started:
					bInputPressing = true;
					break;
				default:
					bInputPressing = false;
				}
			}
		}
	}
}


void USoulGameplayAbility_ChargeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                   bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (ChargingEventDelegate.IsValid())
		GetAbilitySystemComponentFromActorInfo()->UnregisterGameplayTagEvent(ChargingEventDelegate, SoulGameplayTags::Status_Attack_Charging);
}

void USoulGameplayAbility_ChargeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	if (ChargingEventDelegate.IsValid())
		GetAbilitySystemComponentFromActorInfo()->UnregisterGameplayTagEvent(ChargingEventDelegate, SoulGameplayTags::Status_Attack_Charging);
}

bool USoulGameplayAbility_ChargeAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                           const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) &&
		ChargingAbilityClass.GetDefaultObject()->CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void USoulGameplayAbility_ChargeAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	bInputPressing = true;
}

void USoulGameplayAbility_ChargeAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	bInputPressing = false;
	// 차지중 키 릴리즈
	if (GetAbilitySystemComponentFromActorInfo()->GetTagCount(SoulGameplayTags::Status_Attack_Charging) > 0)
	{
		if (USoulAbilitySystemComponent* SoulAbilitySystemComponent = Cast<USoulAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
		{
			OnChargingEnd(SoulAbilitySystemComponent->ChargeRate);
		}
	}
}

TArray<TSubclassOf<USoulGameplayAbility>> USoulGameplayAbility_ChargeAttack::GetChildAbilities() const
{
	TArray Results({ChargingAbilityClass});
	for (const FSoulChargedAttackData& ChargedAttackData : ChargedAttackDatas)
	{
		Results.Add(ChargedAttackData.ChargedAbilityClass);
	}
	return Results;
}

void USoulGameplayAbility_ChargeAttack::OnChargingEnd(float ChargeRate)
{
	for (const FSoulChargedAttackData& ChargedAttackData : ChargedAttackDatas)
	{
		if (ChargedAttackData.ChargeRateRange.Contains(ChargeRate))
		{
			UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
			ActiveAbilityClass = ChargedAttackData.ChargedAbilityClass;
			ASC->TryActivateAbilityByClass(ActiveAbilityClass);
			ASC->OnAbilityEnded.AddUObject(this, &USoulGameplayAbility_ChargeAttack::OnASCAbilityEnded);
			GetAbilitySystemComponentFromActorInfo()->UnregisterGameplayTagEvent(ChargingEventDelegate, SoulGameplayTags::Status_Attack_Charging);
			break;
		}
	}
	SOUL_LOG(Display, TEXT("Charge rate: %f"), ChargeRate);
}

void USoulGameplayAbility_ChargeAttack::OnASCAbilityEnded(const FAbilityEndedData& EndedData)
{
	if (EndedData.AbilityThatEnded.GetClass() == ActiveAbilityClass)
	{
		K2_EndAbility();
	}
}

void USoulGameplayAbility_ChargeAttack::OnChargeTagChanged(const FGameplayTag Tag, int32 Count)
{
	if (Count > 0)
	{
		// 차지 시작과 동시에 끝났음.
		if (!bInputPressing)
		{
			OnChargingEnd(0.f);
		}
	}
	else
	{
		// 차지가 끝났음.
		OnChargingEnd(1.f);
	}
}
