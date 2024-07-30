// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "LockOnPointComponent.generated.h"

#define ECC_LockOnPoint ECollisionChannel::ECC_GameTraceChannel1

/**
 * LockOn의 타겟 지점이 되는 SphereComponent
 */
UCLASS(ClassGroup="LockOn", meta=(DisplayName="LockOn Point", BlueprintSpawnableComponent))
class SOUL_API ULockOnPointComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	ULockOnPointComponent();
	TObjectPtr<class UWidgetComponent> PointWidgetComponent;
};
