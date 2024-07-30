// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility_Jump.h"
#include "SoulGameplayAbility_ChangeMoveSpeed.generated.h"

/**
 * 이동 속도를 변경하는 어빌리티.
 * 걷기, 달리기에 사용.
 */
UCLASS()
class SOUL_API USoulGameplayAbility_ChangeMoveSpeed : public USoulGameplayAbility
{
	GENERATED_BODY()

public:
	USoulGameplayAbility_ChangeMoveSpeed();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	float MoveSpeed = 500.f;
private:
	float SavedSpeed = 0.f;
};
