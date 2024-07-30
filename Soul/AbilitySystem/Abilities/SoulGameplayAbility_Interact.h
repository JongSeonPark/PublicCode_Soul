// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "SoulGameplayAbility_Interact.generated.h"

/**
 * 다른 상호작용 오브젝트와 상호작용하는 어빌리티.
 * IInteractiveObjectInterface 오브젝트와 상호작용.
 */
UCLASS()
class SOUL_API USoulGameplayAbility_Interact : public USoulGameplayAbility
{
	GENERATED_BODY()
	
public:
	USoulGameplayAbility_Interact();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
};
