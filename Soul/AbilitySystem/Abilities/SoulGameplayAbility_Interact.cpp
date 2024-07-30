// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayAbility_Interact.h"

#include "Character/SoulPlayerCharacter.h"
#include "InteractiveObjects/InteractiveObjectInterface.h"

USoulGameplayAbility_Interact::USoulGameplayAbility_Interact()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

bool USoulGameplayAbility_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                       const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) return false;
	if (const ASoulPlayerCharacter* Character = Cast<ASoulPlayerCharacter>(ActorInfo->AvatarActor))
	{
		return Character->GetInteractionActorsInArea().Num() > 0;
	}
	return false;
}

void USoulGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (ASoulPlayerCharacter* Character =  Cast<ASoulPlayerCharacter>(ActorInfo->AvatarActor))
	{
		for (AActor* InteractionObectsInArea : Character->GetInteractionActorsInArea())
		{
			IInteractiveObjectInterface::Execute_Interaction(InteractionObectsInArea, Character);
		}
	}
	K2_EndAbility();
}