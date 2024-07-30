// Copyright 2024 ChickenGames All rights reserved.


#include "SoulEnemyCharacter.h"

#include "Soul/AbilitySystem/SoulAbilitySystemComponent.h"
#include "Soul/AbilitySystem/AttributeSet/SoulAttributeSet.h"
#include "Soul/Combat/LockOn/LockOnComponent.h"

ASoulEnemyCharacter::ASoulEnemyCharacter()
{
	SoulAbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	SoulAbilitySystemComponent->SetIsReplicated(true);
	SoulAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	SoulAttributeSet = CreateDefaultSubobject<USoulAttributeSet>(TEXT("SoulAttributeSet"));
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("LockOnComponent"));
}

void ASoulEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeAbilitySystem();
}

void ASoulEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASoulEnemyCharacter::InitializeAbilitySystem()
{
	SoulAbilitySystemComponent->InitAbilityActorInfo(this, this);
	LockOnComponent->SetAbilitySystemComponent(SoulAbilitySystemComponent);
	Super::InitializeAbilitySystem();
}
