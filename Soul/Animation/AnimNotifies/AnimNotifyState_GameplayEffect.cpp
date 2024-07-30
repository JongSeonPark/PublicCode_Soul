// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotifyState_GameplayEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAnimNotifyState_GameplayEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, 0, ContextHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	}
}

void UAnimNotifyState_GameplayEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
	{
		ASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffect, ASC);
	}
}
