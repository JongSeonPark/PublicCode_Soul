// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SoulPoiseRegenGameplayEffect.generated.h"

/**
 * 맞고나서 10초 후에 발생하는 RegenEffect입니다. 스택을 제한함으로 마지막 공격 이후 10초로 제한함.
 */
UCLASS()
class SOUL_API USoulPoiseRegenGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	USoulPoiseRegenGameplayEffect();
	
};
