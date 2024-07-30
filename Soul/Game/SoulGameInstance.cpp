// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameInstance.h"

#include "Soul.h"
#include "SoulGameplayStatics.h"
#include "Save/SoulSaveGame.h"

void USoulGameInstance::SetSaveSlotName(const FString& InSaveSlotName)
{
	SaveSlotName = InSaveSlotName;
	if (SaveSlotName.IsEmpty())
	{
		SOUL_LOG(Warning, TEXT("SaveSlotName은 빈 값이 될 수 없음."));
		return;
	}
	
	// 로드
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SoulSaveGame = Cast<USoulSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)); 
	}
	else
	{
		SoulSaveGame = Cast<USoulSaveGame>(UGameplayStatics::CreateSaveGameObject(USoulSaveGame::StaticClass()));
		SoulSaveGame->SaveSlotName = SaveSlotName;
		SoulSaveGame->UserIndex = 0;
		UGameplayStatics::SaveGameToSlot(SoulSaveGame, InSaveSlotName, 0);
	}
}

void USoulGameInstance::SaveCurrentGame()
{
	if (SoulSaveGame)
	{
		OnSoulSave.Broadcast(SoulSaveGame);
		USoulGameplayStatics::SoulSaveGameToSlot(SoulSaveGame);
	}
}

void USoulGameInstance::AsyncSaveCurrentGame(FAsyncSaveGameToSlotDelegate SavedDelegate)
{
	if (SoulSaveGame)
	{
		OnSoulSave.Broadcast(SoulSaveGame);
		USoulGameplayStatics::AsyncSoulSaveGameToSlot(SoulSaveGame, SavedDelegate);
	}
}
