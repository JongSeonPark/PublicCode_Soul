// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SoulGameplayAbility.generated.h"

class ASoulPlayerCharacter;
class UInputAction;

UCLASS()
class SOUL_API USoulGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	UInputAction* GetInputAction() const { return InputAction; }
	UInputAction* GetParentInputAction() const { return ParentInputAction; }
	void SetInputAction(UInputAction* InInputAciton) { InputAction = InInputAciton; }
	void SetParentInputAction(UInputAction* InInputAciton) { ParentInputAction = InInputAciton; }

	// 이 어빌리티와 관련된 모든 어빌리티를 반환 
	virtual TArray<TSubclassOf<USoulGameplayAbility>> GetChildAbilities() const { return TArray<TSubclassOf<USoulGameplayAbility>>(); }
	
protected:
	TObjectPtr<UInputAction> InputAction;
	TObjectPtr<UInputAction> ParentInputAction;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	bool bCancelToInputCompleted = false;
	
private:
	TArray<FGameplayAbilitySpecHandle> ChildAbilitySpecHandles;
	
};
