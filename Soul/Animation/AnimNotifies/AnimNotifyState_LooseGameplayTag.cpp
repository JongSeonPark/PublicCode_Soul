// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotifyState_LooseGameplayTag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UAnimNotifyState_LooseGameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetAttachParentActor()))
	{
		AbilitySystemComponent->AddLooseGameplayTags(GameplayTags);
		// if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
		// {
		// 	AbilitySystemComponent->AddReplicatedLooseGameplayTags(GameplayTags);
		// }
	}
}

void UAnimNotifyState_LooseGameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetAttachParentActor()))
	{
		AbilitySystemComponent->RemoveLooseGameplayTags(GameplayTags);
		// if (UAbilitySystemGlobals::Get().ShouldReplicateActivationOwnedTags())
		// {
		// 	AbilitySystemComponent->RemoveReplicatedLooseGameplayTags(GameplayTags);
		// }
	}
}

FString UAnimNotifyState_LooseGameplayTag::GetNotifyName_Implementation() const
{
	if (!GameplayTags.IsEmpty())
	{
		return GameplayTags.ToStringSimple();
	}
	else
	{
		return Super::GetNotifyName_Implementation();
	}
}
