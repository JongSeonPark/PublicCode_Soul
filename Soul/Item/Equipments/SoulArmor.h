// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulEquipment.h"
#include "SoulArmor.generated.h"

USTRUCT(BlueprintType)
struct FArmorMeshData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TObjectPtr<USkeletalMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	FName SocketName;
};

UCLASS()
class SOUL_API ASoulArmor : public ASoulEquipment
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Armor")
	TArray<FArmorMeshData> ArmorMeshDatas;
};
