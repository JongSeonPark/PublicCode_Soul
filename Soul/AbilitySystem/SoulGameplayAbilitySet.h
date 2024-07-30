// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SoulAbilitySystemComponent.h"
#include "SoulGameplayAbilitySet.generated.h"

class UAbilitySystemComponent;
class UInputAction;
class UGameplayAbility;

USTRUCT()
struct FSoulGameplayAbilityBindInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = BindInfo)
	UInputAction* InputAction = nullptr;
	UPROPERTY(EditAnywhere, Category = BindInfo)
	TSubclassOf<UGameplayAbility>	GameplayAbilityClass  = nullptr;
};


/**
 * 캐릭터에 어빌리티를 적용시키기 위한 어빌리티셋. 
 */
UCLASS()
class SOUL_API USoulGameplayAbilitySet : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = BindInfo)
	TArray<FSoulGameplayAbilityBindInfo> Abilities;
	
	UPROPERTY(EditAnywhere, Category = Ability)
	TArray<TSubclassOf<UGameplayAbility>> NoBindAbilties;
public:
	void GiveAbilities(USoulAbilitySystemComponent* AbilitySystemComponent) const;
	void BindInputs(UInputComponent* InputComponent, USoulAbilitySystemComponent* AbilitySystemComponent) const;

	UPROPERTY(EditAnywhere, Category = BindInfo, meta = (Categories = "Input"))
	TMap<FGameplayTag, UInputAction*> BindInputToActivateTags;
};
