// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentDatas.generated.h"

class UInputAction;
class USoulGameplayAbility;

USTRUCT()
struct FSoulWeaponAbilityWithInputAction
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	UInputAction* InputAction = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<USoulGameplayAbility> DefaultAbilityClass;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<USoulGameplayAbility> JumpAbilityClass;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<USoulGameplayAbility> DashAbilityClass;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<USoulGameplayAbility> CriticalAbilityClass;
};

UCLASS()
class SOUL_API USoulEquipmentAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Ability", meta = (Categories = "Input"))
	TArray<FSoulWeaponAbilityWithInputAction> WeaponAbilityDatas;

	// 기타 적용해야할 어빌리티들
	// WallImpact 등
	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<USoulGameplayAbility>> MiscAbilityClasses;

	// 무기 등에서 Animation을 바꿀 때 적용
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimClassLayer;
};
