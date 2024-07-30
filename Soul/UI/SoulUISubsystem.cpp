// Copyright 2024 ChickenGames All rights reserved.


#include "SoulUISubsystem.h"

#include "WidgetControllers/SoulWidgetController.h"

USoulWidgetController* USoulUISubsystem::GetWidgetController(const TSubclassOf<USoulWidgetController> ControllerClass) const
{
	return Controllers.Contains(ControllerClass) ? Controllers[ControllerClass] : nullptr;
}

void USoulUISubsystem::SetWidgetController(USoulWidgetController* WidgetController)
{
	Controllers.Add(WidgetController->GetClass(), WidgetController);
}
