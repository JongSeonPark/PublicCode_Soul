// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotify_RemoveLooseGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"

void UAnimNotify_RemoveLooseGameplayTags::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetAttachParentActor()))
	{
		AbilitySystemComponent->RemoveLooseGameplayTags(GameplayTags);
		if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
		{
			AbilitySystemComponent->RemoveReplicatedLooseGameplayTags(GameplayTags);
		}
	}
}

FString UAnimNotify_RemoveLooseGameplayTags::GetNotifyName_Implementation() const
{
	if (!GameplayTags.IsEmpty())
	{
		return "Remove " + GameplayTags.ToStringSimple();
	}
	else
	{
		return Super::GetNotifyName_Implementation();
	}
}
