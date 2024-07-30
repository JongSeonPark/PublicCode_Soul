// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulAbilitySystemBlueprintLibrary.generated.h"

class USoulAbilitySystemComponent;
class UAbilitySystemComponent;

UCLASS()
class SOUL_API USoulAbilitySystemBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = GameplayEffect)
	static bool TryAttackToTarget(AActor* SourceActor, AActor* TargetActor, float DamageRate, float StanceDamageRate, float PoiseDamageRate);
	
	UFUNCTION(BlueprintPure, Category = Ability, Meta=(DefaultToSelf = "Actor"))
	static USoulAbilitySystemComponent* GetAbilitySystemComponent(const AActor* Actor);

};
