// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item/SoulItemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SoulGameInstance.generated.h"

class USoulSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoulSave, USoulSaveGame*, SoulSaveGame);

/**
 * 
 */
UCLASS()
class SOUL_API USoulGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", Meta = (RequiredAssetDataTags = "RowStructure=/Script/Soul.SoulItemDefinition"))
	TObjectPtr<UDataTable> ItemDefinitions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FSoulQualityColors ItemColors;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FLinearColor GetQualityColor(ESoulItemQuality Quality) const { return ItemColors.QualityColors.Contains(Quality) ? ItemColors.QualityColors[Quality] : FLinearColor::White; }

	UFUNCTION(BlueprintCallable)
	void SetSaveSlotName(const FString& InSaveSlotName);
	UFUNCTION(BlueprintCallable)
	void SaveCurrentGame();
	void AsyncSaveCurrentGame(FAsyncSaveGameToSlotDelegate SavedDelegate);

	UPROPERTY(BlueprintAssignable)
	FOnSoulSave OnSoulSave;

	UPROPERTY(BlueprintReadWrite, Category = "Save")
	FString SaveSlotName;
	UPROPERTY(BlueprintReadWrite, Category = "Save")
	USoulSaveGame* SoulSaveGame;
};
