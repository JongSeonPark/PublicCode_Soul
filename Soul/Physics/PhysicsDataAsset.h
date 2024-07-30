// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FPhysicsEffectData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> NiagaraEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<USoundBase> SoundEffect;
};

/**
 * EPhysicalSurface에 대응하는 SFX, VFX를 정의
 */
UCLASS(BlueprintType)
class SOUL_API UPhysicsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Physics")
	TMap<TEnumAsByte<EPhysicalSurface>, FPhysicsEffectData> EffectToSurface;
};
