// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "SoulAnimInstance.generated.h"

class ASoulCharacterBase;
/**
 * 
 */
UCLASS()
class SOUL_API USoulAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	
	UFUNCTION(BlueprintCallable, Category = "Animation")
	ASoulCharacterBase* TryGetOwningSoulCharacter() const;

	void InitAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent);

private:
	UPROPERTY(EditDefaultsOnly, Category = GameplayTag, meta = (AllowPrivateAccess=true))
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

};
