// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "SoulPlayerState.generated.h"

class USoulAttributeSet;
class USoulAbilitySystemComponent;

/**
 * Player의 AbilitySystemCompoent, AttributeSet을 생성
 */
UCLASS()
class SOUL_API ASoulPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASoulPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	USoulAttributeSet* GetSoulAttributeSet() const { return SoulAttributeSet; }

private:
	UPROPERTY()
	TObjectPtr<USoulAbilitySystemComponent> SoulAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<USoulAttributeSet> SoulAttributeSet;
};
