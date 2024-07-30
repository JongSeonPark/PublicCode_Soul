// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "SoulGameplayAbility_OpenWidget.generated.h"

class USoulUserWidget;

/**
 * 위젯을 여는 어빌리티.
 * 토글 방식으로 진행.
 */
UCLASS()
class SOUL_API USoulGameplayAbility_OpenWidget : public USoulGameplayAbility
{
	GENERATED_BODY()

public:
	USoulGameplayAbility_OpenWidget();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
							bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<USoulUserWidget> WidgetClassToOpen;
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<class USoulWidgetController> WidgetControllerClass;
	TObjectPtr<USoulUserWidget> CreatedWidget;

private:
	FDelegateHandle OnWidgetClosedHandle;
	
};
