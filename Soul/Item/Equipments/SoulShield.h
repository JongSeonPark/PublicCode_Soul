// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SoulEquipment.h"
#include "GameFramework/Actor.h"
#include "SoulShield.generated.h"

UCLASS()
class SOUL_API ASoulShield : public ASoulEquipment
{
	GENERATED_BODY()

public:
	ASoulShield();
	virtual void BeginPlay() override;
	
	virtual void Equip(USoulAbilitySystemComponent* InAbilitySystemComponent) override;
	virtual void Unequip() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shield", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxComponent;

	UFUNCTION()
	void OnGuardTagChanged(const FGameplayTag Tag, int32 Count);
	
	FDelegateHandle GuardTagChangedDelegateHandle;
};
