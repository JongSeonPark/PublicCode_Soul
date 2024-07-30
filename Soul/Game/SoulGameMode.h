// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SoulGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_API ASoulGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSaveGame(USoulSaveGame* SaveGame);
	
};
