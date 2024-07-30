// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulWidgetController.h"
#include "SoulIngameWidgetController.generated.h"


UCLASS()
class SOUL_API USoulIngameWidgetController : public USoulWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindAllEvent() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitValue() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnHealthChange;
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnMaxHealthChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnManaChange;
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnMaxManaChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnStaminaChange;
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnMaxStaminaChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnStanceHealthChange;
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnMaxStanceHealthChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnPoiseHealthChange;
	UPROPERTY(BlueprintAssignable)
	FOnControllerValueChange OnMaxPoiseHealthChange;
};
