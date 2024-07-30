// Copyright 2024 ChickenGames All rights reserved.


#include "SoulPlayerState.h"

#include "Soul/AbilitySystem/SoulAbilitySystemComponent.h"
#include "Soul/AbilitySystem/AttributeSet/SoulAttributeSet.h"

ASoulPlayerState::ASoulPlayerState()
{
	SoulAbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("SoulAbilitySystemComponent"));
	SoulAbilitySystemComponent->SetIsReplicated(true);
	SoulAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	SoulAttributeSet = CreateDefaultSubobject<USoulAttributeSet>(TEXT("SoulAttributeSet"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ASoulPlayerState::GetAbilitySystemComponent() const
{
	return SoulAbilitySystemComponent;
}
