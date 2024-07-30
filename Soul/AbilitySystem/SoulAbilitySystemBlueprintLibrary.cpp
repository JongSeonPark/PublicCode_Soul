// Copyright 2024 ChickenGames All rights reserved.


#include "SoulAbilitySystemBlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "SoulAbilitySystemComponent.h"
#include "SoulGameplayTags.h"
#include "Effects/SoulDamageGameplayEffect.h"

bool USoulAbilitySystemBlueprintLibrary::TryAttackToTarget(AActor* SourceActor, AActor* TargetActor, float DamageRate, float StanceDamageRate, float PoiseDamageRate)
{
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !TargetASC) return false;
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.AddActors({TargetActor});
	const FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(USoulDamageGameplayEffect::StaticClass(), 0, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage, DamageRate);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage_Stance, StanceDamageRate);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage_Poise, PoiseDamageRate);
	SourceASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetASC);
	return true;
}

USoulAbilitySystemComponent* USoulAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(const AActor* Actor)
{
	return Cast<USoulAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor));
}
