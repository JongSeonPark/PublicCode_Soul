// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameMode.h"

#include "SoulGameInstance.h"
#include "SoulGameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Save/SoulSaveGame.h"

void ASoulGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (USoulGameInstance* SoulGameInstance = USoulGameplayStatics::GetSoulGameInstance(GetWorld()))
	{
		if (SoulGameInstance->SoulSaveGame)
		{
			SoulGameInstance->OnSoulSave.AddDynamic(this, &ASoulGameMode::OnSaveGame);
		}
	}
}

void ASoulGameMode::OnSaveGame(USoulSaveGame* SaveGame)
{
	SaveGame->LastLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
}
