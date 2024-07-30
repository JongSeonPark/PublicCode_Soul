// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SoulGameplayStatics.generated.h"

class USoulSaveGame;

UCLASS()
class SOUL_API USoulGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject"))
	static class USoulGameInstance* GetSoulGameInstance(const UObject* WorldContextObject);

	static void AsyncSoulSaveGameToSlot(const FString& SlotName, const int32 UserIndex, FAsyncSaveGameToSlotDelegate SavedDelegate = FAsyncSaveGameToSlotDelegate());
	static void AsyncSoulSaveGameToSlot(USoulSaveGame* SoulSaveGame, FAsyncSaveGameToSlotDelegate SavedDelegate = FAsyncSaveGameToSlotDelegate());
	static void SoulSaveGameToSlot(USoulSaveGame* SoulSaveGame);

	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject"))
	static TArray<USoulSaveGame*> GetAllSaveSlots(); 

};
