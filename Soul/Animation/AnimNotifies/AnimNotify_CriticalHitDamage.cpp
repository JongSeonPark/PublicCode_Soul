// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotify_CriticalHitDamage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SoulGameplayTags.h"
#include "Character/SoulCharacterBase.h"


void UAnimNotify_CriticalHitDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ASoulCharacterBase* Owner = Cast<ASoulCharacterBase>(MeshComp->GetOwner());
	if (!Owner || !Owner->CriticalHitTarget) return;
	if (UAbilitySystemComponent* AbilitySystemComponent = Owner->GetAbilitySystemComponent())
	{
		UAbilitySystemComponent* TargetASC = Owner->CriticalHitTarget->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttackEffectClass, 0, EffectContextHandle);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage, DamageRate);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage_Stance, StanceDamageRate);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage_Poise, PoiseDamageRate);
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetASC);
	}
	
}
