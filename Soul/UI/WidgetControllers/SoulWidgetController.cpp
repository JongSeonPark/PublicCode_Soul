// Copyright 2024 ChickenGames All rights reserved.


#include "SoulWidgetController.h"

void USoulWidgetController::InitData(FControllerParams Params)
{
	Character = Params.Character;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	BindAllEvent();
}
