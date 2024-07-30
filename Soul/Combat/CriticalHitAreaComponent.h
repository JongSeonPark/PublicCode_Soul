// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CriticalHitAreaComponent.generated.h"

UENUM()
enum class ECriticalHitAreaType { Front, Back };

/**
 * Critical Hit 타겟의 지점이 되는 컴포넌트
 */
UCLASS(ClassGroup=Combat, meta=(BlueprintSpawnableComponent))
class SOUL_API UCriticalHitAreaComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	ECriticalHitAreaType GetCriticalHitAreaType() const { return CriticalHitAreaType; }
	bool IsActiveWhenBeginPlay() const { return bIsActiveWhenBeginPlay; }
	virtual void SetActive(bool bNewActive, bool bReset = false) override;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "CriticalHit", meta=(AllowPrivateAccess=true))
	ECriticalHitAreaType CriticalHitAreaType;

	// Active 초창기 세팅.
	UPROPERTY(EditDefaultsOnly, Category = "CriticalHit", meta=(AllowPrivateAccess=true))
	bool bIsActiveWhenBeginPlay;
};
