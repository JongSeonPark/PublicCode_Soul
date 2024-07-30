// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayAbility_ChangeMoveSpeed.h"

#include "Character/SoulCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

USoulGameplayAbility_ChangeMoveSpeed::USoulGameplayAbility_ChangeMoveSpeed()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bCancelToInputCompleted = true;
}

void USoulGameplayAbility_ChangeMoveSpeed::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo) || !CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	if (ASoulCharacterBase* Character = Cast<ASoulCharacterBase>(ActorInfo->AvatarActor))
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
 		SavedSpeed = MovementComponent->MaxWalkSpeed;
		MovementComponent->MaxWalkSpeed = MoveSpeed;
	}
}

void USoulGameplayAbility_ChangeMoveSpeed::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (ASoulCharacterBase* Character = Cast<ASoulCharacterBase>(ActorInfo->AvatarActor))
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
		MovementComponent->MaxWalkSpeed = SavedSpeed;
		SavedSpeed = 0.f;
	}
}
