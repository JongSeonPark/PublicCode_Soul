// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SoulWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerValueChange, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerBooleanValueChange, bool, Value);
class USoulAbilitySystemComponent;
class ASoulCharacterBase;

USTRUCT()
struct FControllerParams
{
	GENERATED_BODY()

	TObjectPtr<ASoulCharacterBase> Character = nullptr;
	TObjectPtr<USoulAbilitySystemComponent> AbilitySystemComponent = nullptr;
};

UCLASS(Abstract, editinlinenew, BlueprintType, Blueprintable, meta=( DontUseGenericSpawnObject="True", DisableNativeTick))
class SOUL_API USoulWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USoulAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ASoulCharacterBase> Character;
	virtual void InitData(FControllerParams Params);

	virtual void BroadcastInitValue() {}
	virtual void BindAllEvent() {}
	
};
