// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/SoulAbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "Item/SoulItemTypes.h"
#include "SoulEquipment.generated.h"

class USoulEquipmentAbilityDataAsset;
class UAbilitySystemComponent;
class USoulAttackDataAsset;

UCLASS()
class SOUL_API ASoulEquipment : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Equip(USoulAbilitySystemComponent* InAbilitySystemComponent);
	virtual void Unequip();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	void SetEquipmentSlotType(const ESoulEquipmentSlotType& InEquipmentSlotType) { EquipmentSlotType = InEquipmentSlotType; }
	ESoulEquipmentSlotType GetEquipmentSlotType() const { return EquipmentSlotType; }

protected:
	TObjectPtr<USoulAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipment", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USoulEquipmentAbilityDataAsset> EquipmentAbilityDataAsset;

	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	
	ESoulEquipmentSlotType EquipmentSlotType; 
};
