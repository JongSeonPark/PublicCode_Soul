// Copyright 2024 ChickenGames All rights reserved.


#include "SoulPoiseRegenGameplayEffect.h"

#include "AbilitySystem/AttributeSet/SoulAttributeSet.h"

USoulPoiseRegenGameplayEffect::USoulPoiseRegenGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FGameplayEffectModifierMagnitude(10.f);
	Period = 10.f;
	bExecutePeriodicEffectOnApplication = false;

	FGameplayModifierInfo PoiseModifierInfo;
	PoiseModifierInfo.Attribute = USoulAttributeSet::GetPoiseHealthAttribute();
	PoiseModifierInfo.ModifierOp = EGameplayModOp::Override;
	FAttributeBasedFloat PoiseAttributeBasedFloat = FAttributeBasedFloat();
	PoiseAttributeBasedFloat.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(USoulAttributeSet::GetMaxPoiseHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	PoiseModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(PoiseAttributeBasedFloat);
	Modifiers.Add(PoiseModifierInfo);
	
	FGameplayModifierInfo StanceModifierInfo;
	StanceModifierInfo.Attribute = USoulAttributeSet::GetStanceHealthAttribute();
	StanceModifierInfo.ModifierOp = EGameplayModOp::Override;
	FAttributeBasedFloat StanceAttributeBasedFloat = FAttributeBasedFloat();
	StanceAttributeBasedFloat.BackingAttribute = FGameplayEffectAttributeCaptureDefinition(USoulAttributeSet::GetMaxStanceHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	StanceModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(StanceAttributeBasedFloat);
	Modifiers.Add(StanceModifierInfo);

	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1.f;
}
