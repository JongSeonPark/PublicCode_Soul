// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_PlayMontage.h"
#include "SoulGameplayAbility_ChargeAttack.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_PlayMontageAndWait;

/**
 * 강공격 차지에 있어 차징 비율에 따라 공격이 달라진다. 
 */
USTRUCT()
struct FSoulChargedAttackData
{
	GENERATED_BODY()

	// 이거는 AttackAbility에 UAnimNotifyState_Charging가 있는 경우 사용
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<USoulGameplayAbility> ChargedAbilityClass;

	// FFloatRange 범위 한정 metaData가 없는게 아쉽다.
	// [0, 1] 구간이어야 함! 폐구간(Include, 0과 1을 포함.)
	UPROPERTY(EditDefaultsOnly, Category = "Charge")
	FFloatRange ChargeRateRange;

	bool IsEmpty() const { return ChargedAbilityClass == nullptr; }
};

/**
 * 차지 공격을 위한 어빌리티
 */
UCLASS()
class SOUL_API USoulGameplayAbility_ChargeAttack : public USoulGameplayAbility
{
	GENERATED_BODY()

public:
	USoulGameplayAbility_ChargeAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<USoulGameplayAbility> ChargingAbilityClass;
	UPROPERTY(EditAnywhere, Category = "Attack")
	TArray<FSoulChargedAttackData> ChargedAttackDatas;

	virtual TArray<TSubclassOf<USoulGameplayAbility>> GetChildAbilities() const override;

private:
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent;
	TSubclassOf<USoulGameplayAbility> ActiveAbilityClass;

	void OnChargingEnd(float ChargeRate);
	void OnASCAbilityEnded(const FAbilityEndedData& EndedData);

	void OnChargeTagChanged(const FGameplayTag Tag, int32 Count);

	bool bInputPressing = true;

	FDelegateHandle ChargingEventDelegate;
};
