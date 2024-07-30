// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility_PlayMontage.h"
#include "GameplayAbility_CriticalHit.generated.h"

/**
 * 상대가 붕괴 상태이거나 뒤잡기가 가능한 상태일 때 치명적인 공격을 하는 어빌리티
 * UCriticalHitAreaComponent가 Overlap인 상태에서 작동함.
 */
UCLASS()
class SOUL_API UGameplayAbility_CriticalHit : public UGameplayAbility_PlayMontage
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_CriticalHit();
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo,
							 const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	TObjectPtr<UAnimMontage> BeCriticalMontageToPlay;
	
	TArray<UPrimitiveComponent*> GetOverlapComponents(AActor* Avatar) const;
};
