// Copyright 2024 ChickenGames All rights reserved.


#include "AbilitySystem/Effects/SoulDamageGameplayEffect.h"

#include "SoulGameplayTags.h"
#include "SoulPoiseRegenGameplayEffect.h"
#include "AbilitySystem/Executions/SoulDamageExecution.h"
#include "GameplayEffectComponents/AdditionalEffectsGameplayEffectComponent.h"

USoulDamageGameplayEffect::USoulDamageGameplayEffect()
{
	FGameplayEffectExecutionDefinition ExecutionDefinition;
	ExecutionDefinition.CalculationClass = USoulDamageExecution::StaticClass();
	Executions.Push(ExecutionDefinition);
	
	FGameplayEffectCue Cue;
	Cue.GameplayCueTags = FGameplayTagContainer(SoulGameplayTags::GameplayCue_Damage); 
	GameplayCues.Add(Cue);

	// 맞은 후에 10초후에 발생하는 PoiseRegen을 구현하기 위함. 
	UAdditionalEffectsGameplayEffectComponent* AdditionalEffectsGameplayEffectComponent = CreateDefaultSubobject<UAdditionalEffectsGameplayEffectComponent>("Apply Additional Effects");
	AdditionalEffectsGameplayEffectComponent->OnApplicationGameplayEffects.Add(FConditionalGameplayEffect(USoulPoiseRegenGameplayEffect::StaticClass()));
	GEComponents.Add(AdditionalEffectsGameplayEffectComponent);
}