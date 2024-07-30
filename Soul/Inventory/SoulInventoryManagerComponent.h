// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulEquipmentInventoryComponent.h"
#include "Components/ActorComponent.h"
#include "Item/SoulItemTypes.h"
#include "SoulInventoryManagerComponent.generated.h"


class ASoulCharacterBase;

UCLASS()
class SOUL_API USoulInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void TryUseItem(FName ItemID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItem(FName EquipItemID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UnequipItem(ESoulEquipmentSlotType ItemSlot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	ASoulEquipment* GetEquipmentItem(ESoulEquipmentSlotType EquipmentSlotType);

	UFUNCTION(Server, Reliable)
	void AddToItems_Server(const TArray<FSoulItemData>& Items, AActor* ActorToDestroy = nullptr);
	
private:
	TObjectPtr<USoulEquipmentInventoryComponent> EquipmentInventoryComponent;

};
