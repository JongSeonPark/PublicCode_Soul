// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotify_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, Payload);
}

FString UAnimNotify_SendGameplayEvent::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		return "Send EventTag: " + EventTag.ToString();
	}
	else
	{
		return Super::GetNotifyName_Implementation();
	}
}
