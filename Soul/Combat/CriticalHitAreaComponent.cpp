// Copyright 2024 ChickenGames All rights reserved.


#include "CriticalHitAreaComponent.h"

void UCriticalHitAreaComponent::SetActive(bool bNewActive, bool bReset)
{
	Super::SetActive(bNewActive, bReset);
	SetVisibleFlag(bNewActive);
	SetCollisionEnabled(bNewActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void UCriticalHitAreaComponent::BeginPlay()
{
	Super::BeginPlay();
	SetActive(bIsActiveWhenBeginPlay);
}


