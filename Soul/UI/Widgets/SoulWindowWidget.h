// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulUserWidget.h"
#include "SoulWindowWidget.generated.h"

/**
 * 여닫기 위한 WindowWidget
 */
UCLASS()
class SOUL_API USoulWindowWidget : public USoulUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void CloseWidget() override;
};
