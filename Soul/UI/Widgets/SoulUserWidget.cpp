// Copyright 2024 ChickenGames All rights reserved.


#include "SoulUserWidget.h"

void USoulUserWidget::SetWidgetController(USoulWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnChangeController(InWidgetController);
}

void USoulUserWidget::CloseWidget()
{
	RemoveFromParent();
	OnWidgetClosed.Broadcast();
	// 즉각적인 UI 제거를 위해 CollectGarbage를 호출
	CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
}
