// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SoulGameplayAbility.h"
#include "SoulComboAbility.generated.h"

/**
 * 콤보 발생용 어빌리티
 */
UCLASS()
class SOUL_API USoulComboAbility : public USoulGameplayAbility
{
	GENERATED_BODY()

public:
	USoulComboAbility();
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	                                const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual TArray<TSubclassOf<USoulGameplayAbility>> GetChildAbilities() const override;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<USoulGameplayAbility>> ComboAbilityClasses;

private:
	void OnASCAbilityActivated(UGameplayAbility* Ability);
	void OnASCAbilityEnded(const FAbilityEndedData& EndedData);

	FGameplayAbilitySpec ComboAbilitySpec;
};
