// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotifyState_SetMovementMode.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Character/SoulPlayerCharacter.h"

void UAnimNotifyState_SetMovementMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (ASoulPlayerCharacter* PlayerCharacter = Cast<ASoulPlayerCharacter>(MeshComp->GetAttachParentActor()))
	{
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(NewMovementMode, NewCustomMode);
	}
}

void UAnimNotifyState_SetMovementMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (ASoulPlayerCharacter* PlayerCharacter = Cast<ASoulPlayerCharacter>(MeshComp->GetAttachParentActor()))
	{
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking, 0);
	}
}
