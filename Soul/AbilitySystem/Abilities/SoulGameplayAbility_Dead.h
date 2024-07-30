// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "Animation/SoulAnimDatas.h"
#include "SoulGameplayAbility_Dead.generated.h"

/**
 * 죽음 상태 어빌리티.
 */
UCLASS()
class SOUL_API USoulGameplayAbility_Dead : public USoulGameplayAbility
{
	GENERATED_BODY()

public:
	USoulGameplayAbility_Dead();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	FPlayMontageData MontageData;

private:
	UFUNCTION()
	void OnFinishAnim();
};
