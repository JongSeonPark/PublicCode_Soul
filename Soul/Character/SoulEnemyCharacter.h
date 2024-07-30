// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulCharacterBase.h"
#include "SoulEnemyCharacter.generated.h"

UCLASS()
class SOUL_API ASoulEnemyCharacter : public ASoulCharacterBase
{
	GENERATED_BODY()

public:
	ASoulEnemyCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void InitializeAbilitySystem() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LockOn")
	TObjectPtr<class ULockOnComponent> LockOnComponent;

};
