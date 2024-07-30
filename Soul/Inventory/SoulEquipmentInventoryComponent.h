// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulInventoryComponent.h"
#include "SoulEquipmentInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentChangeDelegate, ESoulEquipmentSlotType, EquipmentSlotType);

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class SOUL_API USoulEquipmentInventoryComponent : public USoulInventoryComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItem(FName EquipItemID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UnequipItem(ESoulEquipmentSlotType ItemSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ASoulEquipment* GetEquipmentItem(ESoulEquipmentSlotType EquipmentSlotType) { return EquippedItems.Contains(EquipmentSlotType) ? EquippedItems[EquipmentSlotType].Equipment : nullptr; }

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChangeDelegate EquipmentChangeDelegate;

protected:
	virtual void OnSaveGame(USoulSaveGame* SaveGame) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = true))
	TMap<ESoulEquipmentSlotType, FEquipmentInventoryData> EquippedItems;

	UFUNCTION(Server, Reliable)
	void EquipItem_Server(FName EquipItemID);
	UFUNCTION(NetMulticast, Reliable)
	void EquipItem_Multicast(FName EquipItemID);
	UFUNCTION(Server, Reliable)
	void UnequipItem_Server(ESoulEquipmentSlotType EquipmentSlotType);
	UFUNCTION(NetMulticast, Reliable)
	void UnequipItem_Multicast(ESoulEquipmentSlotType EquipmentSlotType);
};
