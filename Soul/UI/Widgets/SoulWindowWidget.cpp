// Copyright 2024 ChickenGames All rights reserved.


#include "SoulWindowWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void USoulWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, this);
		PlayerController->SetShowMouseCursor(true);
	}
}

void USoulWindowWidget::CloseWidget()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
		PlayerController->SetShowMouseCursor(false);
	}
	Super::CloseWidget();
}
