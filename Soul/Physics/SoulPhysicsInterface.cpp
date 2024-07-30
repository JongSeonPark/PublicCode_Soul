// Copyright 2024 ChickenGames All rights reserved.


#include "Physics/SoulPhysicsInterface.h"

bool ISoulPhysicsInterface::TryGetPhysicsEffectData(TEnumAsByte<EPhysicalSurface> SurfaceType, FPhysicsEffectData& PhysicsEffectData) const
{
	if (UPhysicsDataAsset* PhysicsDataAsset = GetPhysicsDataAsset())
	{
		if (PhysicsDataAsset->EffectToSurface.Contains(SurfaceType))
		{
			PhysicsEffectData = PhysicsDataAsset->EffectToSurface[SurfaceType];
			return true;
		}		
	}

	return false;
}
