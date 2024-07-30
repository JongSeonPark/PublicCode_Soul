// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulWidgetController.h"
#include "SoulWorldHealthBarController.generated.h"

UCLASS()
class SOUL_API USoulWorldHealthBarController : public USoulWidgetController
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
	FOnControllerValueChange OnDamage;

	UPROPERTY(BlueprintAssignable)
	FOnControllerBooleanValueChange OnTargeted;
};
