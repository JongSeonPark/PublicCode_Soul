// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_PlayMontage.h"
#include "Item/SoulItemTypes.h"
#include "GameplayAbility_PlayConsumeItemMontage.generated.h"

class UAbilityTask_WaitGameplayEvent;
/**
 * 애니메이션을 실행하면서 Consumable 아이템 사용하는 어빌리티.
 */
UCLASS()
class SOUL_API UGameplayAbility_PlayConsumeItemMontage : public UGameplayAbility_PlayMontage
{
	GENERATED_BODY()

public:
	UGameplayAbility_PlayConsumeItemMontage();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FName ItemID;
		
	UPROPERTY(EditDefaultsOnly, Category = "GameplayAbility | Effect")
	TArray<TSubclassOf<UGameplayEffect>> AdjustEffects;

	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	FPlayMontageData PlayMontageData_Fail;

private:
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = nullptr;
	int32 Amount = 0;

	UFUNCTION()
	void OnConsumeEvent(FGameplayEventData Payload);

};
