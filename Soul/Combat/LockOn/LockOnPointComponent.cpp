// Copyright 2024 ChickenGames All rights reserved.


#include "LockOnPointComponent.h"

#include "Components/WidgetComponent.h"
#include "Soul/Soul.h"

ULockOnPointComponent::ULockOnPointComponent()
{
	BodyInstance.SetObjectType(ECC_LockOnPoint);
	BodyInstance.SetResponseToAllChannels(ECR_Ignore);
	BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BodyInstance.SetResponseToChannel(ECC_LockOnPoint, ECR_Block);
	SetGenerateOverlapEvents(false);
	// PointWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	// PointWidgetComponent->SetupAttachment(this);
}
