// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsDataAsset.h"
#include "UObject/Interface.h"
#include "SoulPhysicsInterface.generated.h"

UINTERFACE(MinimalAPI)
class USoulPhysicsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Physics를 발생시키는 오브젝트의 인터페이스
 */
class SOUL_API ISoulPhysicsInterface
{
	GENERATED_BODY()

public:
	virtual UPhysicsDataAsset* GetPhysicsDataAsset() const = 0;
	virtual bool TryGetPhysicsEffectData(TEnumAsByte<EPhysicalSurface> SurfaceType, FPhysicsEffectData& PhysicsEffectData) const;

};
