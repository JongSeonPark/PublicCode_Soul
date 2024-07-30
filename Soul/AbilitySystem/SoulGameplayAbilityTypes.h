// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "AbilitySystemComponent.h"
#include "SoulGameplayAbilityTypes.generated.h"


USTRUCT()
struct FBufferedAbilityData
{
	GENERATED_BODY()
	FBufferedAbilityData()
	{
	}

	FBufferedAbilityData(FGameplayAbilitySpecHandle AbilitySpecHandle, bool bAllowRemoteActivation)
		: AbilitySpecHandle(AbilitySpecHandle), bAllowRemoteActivation(bAllowRemoteActivation)
	{
	}

	FGameplayAbilitySpecHandle AbilitySpecHandle;
	bool bAllowRemoteActivation;

	bool IsValid() const
	{
		return AbilitySpecHandle.IsValid();
	}
};