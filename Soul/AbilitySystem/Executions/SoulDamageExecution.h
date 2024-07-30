// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "SoulDamageExecution.generated.h"

/**
 * Damage 및 Groggy Execution.
 */
UCLASS()
class SOUL_API USoulDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	USoulDamageExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	void ExecuteDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	void ExecuteGuardDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

private:
	// 다크소울 방식의 대미지 계산
	float CalculateDamage(float Damage, float Defence) const;
	
};
