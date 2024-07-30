// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SoulAnimDatas.generated.h"

USTRUCT()
struct FPlayMontageData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	TObjectPtr<UAnimMontage> MontageToPlay;
	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	float PlayRate = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	FName SectionName = NAME_None;
};
