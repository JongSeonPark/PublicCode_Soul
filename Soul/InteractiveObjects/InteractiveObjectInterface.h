// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractiveObjectInterface.generated.h"

class ASoulCharacterBase;

UINTERFACE()
class UInteractiveObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 플레이어와 상호작용 하는 오브젝트에 적용되는 인터페이스
 * SoulGameplayAbility_Interact에서 사용됨
 */
class SOUL_API IInteractiveObjectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interaction(ASoulCharacterBase* InteractionCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetInteractionInfo();
};
