// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SoulUISubsystem.generated.h"

class USoulWidgetController;

/**
 * WidgetController들을 관리하기 위한 Subsystem 
 */
UCLASS()
class SOUL_API USoulUISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	USoulWidgetController* GetWidgetController(TSubclassOf<USoulWidgetController> ControllerClass) const;
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(USoulWidgetController* WidgetController);

	template<class T>
	T* GetOrAddWidgetController(TSubclassOf<T> ControllerClass = nullptr);

	TMap<UClass*, USoulWidgetController*> Controllers;
};

template <class T>
T* USoulUISubsystem::GetOrAddWidgetController(TSubclassOf<T> ControllerClass)
{
	USoulWidgetController* FindedWidgetController;
	if (ControllerClass)
		FindedWidgetController = GetWidgetController(ControllerClass);
	else
		FindedWidgetController = GetWidgetController(T::StaticClass());
	
	if (FindedWidgetController)
	{
		return Cast<T>(FindedWidgetController);
	}
	else
	{
		T* WidgetController = NewObject<T>();
		SetWidgetController(WidgetController);
		return WidgetController;
	}
}
