// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoulUserWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWidgetClosed)

class USoulWidgetController;

/**
 * WidgetController를 통해 데이터를 사용하는 Widget 
 */
UCLASS()
class SOUL_API USoulUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(USoulWidgetController* InWidgetController);

	UFUNCTION(BlueprintCallable)
	USoulWidgetController* GetWidgetController() const { return WidgetController; }
	
	UFUNCTION(BlueprintCallable)
	virtual void CloseWidget();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeController(USoulWidgetController* InWidgetController);

	FOnWidgetClosed OnWidgetClosed;
	
protected:
	USoulWidgetController* WidgetController;
};
