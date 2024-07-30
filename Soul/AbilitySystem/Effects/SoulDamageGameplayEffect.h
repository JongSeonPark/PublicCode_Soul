// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SoulDamageGameplayEffect.generated.h"

/**
 * 보통 공격을 USoulDamageExecution으로 실행할 것이기 때문에, USoulDamageExecution를 포함한 기본 Effect 생성.
 */
UCLASS()
class SOUL_API USoulDamageGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	USoulDamageGameplayEffect();
	
};