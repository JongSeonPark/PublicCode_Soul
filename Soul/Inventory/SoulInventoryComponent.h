// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/SoulItemTypes.h"
#include "SoulInventoryComponent.generated.h"

// DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddItemDelegate, TArray<FSoulItemData> ItemDatas);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeItemAmountDelegate, const TArray<FSoulItemData>&, ItemDatas);

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class SOUL_API USoulInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoulInventoryComponent();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void AddItem(FSoulItemData ItemData);
	void AddItems(const TArray<FSoulItemData>& ItemDatas);
	void RemoveItem(FSoulItemData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ConsumeItem(FName ItemID, int32 Amount = 1);

	UPROPERTY(BlueprintAssignable)	
	FOnChangeItemAmountDelegate OnAddItemDelegate;
	UPROPERTY(BlueprintAssignable)	
	FOnChangeItemAmountDelegate OnRemoveItemDelegate;
	FSoulItemDefinition* GetItemDefinition(FName EquipItemID);
	TMap<FName, FSoulItemData> GetInventoryItemMap() const { return ItemMap; }

protected:
	// TMap은 Replicate가 되지 않음. InventoryItems의 캐싱용으로 사용.
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	TMap<FName, FSoulItemData> ItemMap;
	UPROPERTY(ReplicatedUsing = OnRep_InventoryItems, BlueprintReadOnly, Category = "Item")
	TArray<FSoulItemData> InventoryItems;
	
	UFUNCTION()
	virtual void OnRep_InventoryItems(const TArray<FSoulItemData>& Old_InventoryItem);
	
	UFUNCTION(Client, Reliable)
	void OnAddItems_Client(const TArray<FSoulItemData>& ItemDatas);
	UFUNCTION(Client, Reliable)
	void OnRemoveItems_Client(const TArray<FSoulItemData>& ItemDatas);

	UFUNCTION()
	virtual void OnSaveGame(USoulSaveGame* SaveGame);
};
