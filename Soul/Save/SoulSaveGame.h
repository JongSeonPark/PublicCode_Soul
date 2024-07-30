// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Item/SoulItemTypes.h"
#include "SoulSaveGame.generated.h"

UCLASS()
class SOUL_API USoulSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	USoulSaveGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
	FString PlayerName;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
	FString SaveSlotName;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
	int32 UserIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
	bool bInitGame = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
	FString LastLevelName;

	// 문을 통해 레벨 이동시 문의 키 정의
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
	FName DoorKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Basic")
	FTransform LastTransform;
	
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	TArray<FSoulItemData> InventoryItems;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	TMap<ESoulEquipmentSlotType, FEquipmentInventoryData> EquippedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attribute")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attribute")
	float StanceHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attribute")
	float PoiseHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attribute")
	float Mana;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attribute")
	float Stamina;
	
};
