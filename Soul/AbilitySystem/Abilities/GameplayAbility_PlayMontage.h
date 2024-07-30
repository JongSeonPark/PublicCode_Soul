// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "Animation/SoulAnimDatas.h"
#include "GameplayAbility_PlayMontage.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * MontageAnimation을 실행시키는 어빌리티.
 */
UCLASS()
class SOUL_API UGameplayAbility_PlayMontage : public USoulGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_PlayMontage();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	FPlayMontageData PlayMontageData;
	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassesWhileAnimating;

	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;

protected:
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask;
	
};
