// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayStatics.h"

#include "Game/SoulGameInstance.h"
#include "Save/SoulSaveGame.h"

USoulGameInstance* USoulGameplayStatics::GetSoulGameInstance(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World ? CastChecked<USoulGameInstance>(World->GetGameInstance()) : nullptr;
}

void USoulGameplayStatics::AsyncSoulSaveGameToSlot(const FString& SlotName, const int32 UserIndex, FAsyncSaveGameToSlotDelegate SavedDelegate)
{
	if (USoulSaveGame* SaveGameInstance = Cast<USoulSaveGame>(UGameplayStatics::CreateSaveGameObject(USoulSaveGame::StaticClass())))
	{
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotName, UserIndex, SavedDelegate);
	}
}

void USoulGameplayStatics::AsyncSoulSaveGameToSlot(USoulSaveGame* SoulSaveGame, FAsyncSaveGameToSlotDelegate SavedDelegate)
{
	UGameplayStatics::AsyncSaveGameToSlot(SoulSaveGame, SoulSaveGame->SaveSlotName, SoulSaveGame->UserIndex, SavedDelegate);
}

void USoulGameplayStatics::SoulSaveGameToSlot(USoulSaveGame* SoulSaveGame)
{
	UGameplayStatics::SaveGameToSlot(SoulSaveGame, SoulSaveGame->SaveSlotName, SoulSaveGame->UserIndex);
}

TArray<USoulSaveGame*> USoulGameplayStatics::GetAllSaveSlots()
{
	TArray<USoulSaveGame*> Results;
	TArray<FString> FoundFiles;
	const FString& ext = ".sav";
	const FString& SaveDir = UKismetSystemLibrary::GetProjectSavedDirectory() + "SaveGames";

	IFileManager::Get().FindFiles(FoundFiles, *SaveDir, *ext);

	TArray<FString> FileNames;
	if (FoundFiles.Num() > 0)
	{
		for (const FString& F : FoundFiles)
		{
			FileNames.AddUnique(F.LeftChop(4));
		}
	}

	if (FileNames.Num() > 0)
	{
		for (const FString& F : FileNames)
		{
			if (USoulSaveGame* SaveGame = Cast<USoulSaveGame>(UGameplayStatics::LoadGameFromSlot(F, 0)))
			{
				Results.Add(SaveGame);
			}
		}
	}

	return Results;
}
