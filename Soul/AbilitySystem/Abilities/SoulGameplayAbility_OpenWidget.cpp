// Copyright 2024 ChickenGames All rights reserved.


#include "SoulGameplayAbility_OpenWidget.h"

#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/SoulUISubsystem.h"
#include "UI/Widgets/SoulUserWidget.h"
#include "UI/WidgetControllers/SoulWidgetController.h"

USoulGameplayAbility_OpenWidget::USoulGameplayAbility_OpenWidget()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;
}

void USoulGameplayAbility_OpenWidget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	FGameplayAbilitySpec* AbilitySpec = GetCurrentAbilitySpec();
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySpec->ActiveCount > 1)
	{
		AbilitySystemComponent->CancelAbilityHandle(AbilitySpec->Handle);
	}
	else
	{
		CreatedWidget = CreateWidget<USoulUserWidget>(GetWorld(), WidgetClassToOpen);
		if (CreatedWidget)
		{
			if (WidgetControllerClass)
			{
				if (USoulUISubsystem* UISubsystem = GetWorld()->GetSubsystem<USoulUISubsystem>())
				{
					USoulWidgetController* WidgetController = UISubsystem->GetOrAddWidgetController(WidgetControllerClass);
					UClass* C = WidgetControllerClass ? WidgetControllerClass->GetClass() : USoulWidgetController::StaticClass(); 
					CreatedWidget->SetWidgetController(WidgetController);
				}
			}
			CreatedWidget->AddToViewport();
			OnWidgetClosedHandle = CreatedWidget->OnWidgetClosed.AddUObject(this, &USoulGameplayAbility_OpenWidget::K2_EndAbility);
		}
	}
}

void USoulGameplayAbility_OpenWidget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (CreatedWidget)
	{
		CreatedWidget->OnWidgetClosed.Remove(OnWidgetClosedHandle);
		CreatedWidget->CloseWidget();
		CreatedWidget = nullptr;
	}
}

void USoulGameplayAbility_OpenWidget::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	if (CreatedWidget)
	{
		CreatedWidget->OnWidgetClosed.Remove(OnWidgetClosedHandle);
		CreatedWidget->CloseWidget();
		CreatedWidget = nullptr;
	}
}

