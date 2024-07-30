// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "SoulGameplayAbilityTypes.h"
#include "Abilities/SoulGameplayAbility.h"
#include "SoulAbilitySystemComponent.generated.h"

class ASoulPlayerCharacter;

/**
 * 어빌리티를 버퍼시키고 Input을 EnhancedInput에 적용되도록 함.
 * TODO: 현재 UInputAction을 SoulGameplayAbility CDO에 넣어 사용하고 있는데, 필요시 구조를 변경.
 */
UCLASS()
class SOUL_API USoulAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryBufferAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryBufferAbilityByClass(TSubclassOf<UGameplayAbility> InAbilityToActivate, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool TryBufferAbility(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ActivateAbilityBuffer();

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	virtual void OnTagUpdated(const FGameplayTag& Tag, bool TagExists) override;

	FGameplayTag GetLastAbilityInputKey() const { return LastAbilityInputKey; }

	void AbilityLocalInputTriggered(UInputAction* InputAction);
	void AbilityLocalInputCompleted(UInputAction* InputAction);
	
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	FGameplayAbilitySpec BuildSoulAbilitySpecFromClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 0, UInputAction* InputAction = nullptr);
	UInputAction* GetInputActionToInputTag(const FGameplayTag& InputTag) { return BindInputToActivateTags.Contains(InputTag) ? BindInputToActivateTags[InputTag] : nullptr; }
	void SetBindInputToActivateTags(const TMap<FGameplayTag, UInputAction*>& InBindInputToActivateTags) { BindInputToActivateTags = InBindInputToActivateTags; }

private:
	// 현재 선입력(버퍼)된 어빌리티
	FBufferedAbilityData BufferedAbilityData;
	TObjectPtr<ASoulPlayerCharacter> SoulAvatarCharacter;

	// 마지막으로 사용된 스킬의 키
	FGameplayTag LastAbilityInputKey;

	TMap<FGameplayTag, UInputAction*> BindInputToActivateTags;
	TSet<UInputAction*> TriggeredActions;
public:
	float ChargeRate;
};
