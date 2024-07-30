// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Physics/SoulPhysicsInterface.h"
#include "SoulCharacterBase.generated.h"

class USoulAttributeSet;
class USoulEquipmentInventoryComponent;
class USoulInventoryManagerComponent;
class UPhysicsDataAsset;
class UGameplayEffect;
class USoulAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitySystemInitialized, UAbilitySystemComponent*, AbilitySystemComponent);

/**
 * 인간형 캐릭터 베이스
 */
UCLASS()
class SOUL_API ASoulCharacterBase : public ACharacter, public IAbilitySystemInterface, public ISoulPhysicsInterface
{
	GENERATED_BODY()

public:
	ASoulCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	USoulAbilitySystemComponent* GetSoulAbilitySystemComponent() const { return SoulAbilitySystemComponent; }
	USoulInventoryManagerComponent* GetInventoryManagerComponent() const { return InventoryManagerComponent; }
	USoulEquipmentInventoryComponent* GetEquipmentInventoryComponent() const { return EquipmentInventoryComponent; }

	virtual UPhysicsDataAsset* GetPhysicsDataAsset() const override { return PhysicsDataAsset; }
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

protected:
	UPROPERTY()
	TObjectPtr<USoulAbilitySystemComponent> SoulAbilitySystemComponent;
	UPROPERTY()
	USoulAttributeSet* SoulAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCharacterTrajectoryComponent> CharacterTrajectory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HealthBarWidgetComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USoulEquipmentInventoryComponent> EquipmentInventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USoulInventoryManagerComponent> InventoryManagerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayAbility)
	TObjectPtr<class USoulGameplayAbilitySet> AbilitySet;

	UPROPERTY(BlueprintAssignable, Category = "AbilitySystem")
	FOnAbilitySystemInitialized OnAbilitySystemInitialized;
	
	virtual void InitializeAbilitySystem();
	virtual void ApplyStartupEffectsToSelf();
	virtual void AddCharacterAbilities();
	
	virtual void OnDying(const FGameplayTag Tag, int32 Count);
	virtual void OnDead(const FGameplayTag Tag, int32 Count);

private:
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem | Effect", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<class USoulUserWidget> HealthBarWidgetClass;
	void TryCreateHealthBar();
	void CreateWorldHealthBarWidget();
	TObjectPtr<class USoulWorldHealthBarController> HealthBarController;

	UPROPERTY(EditDefaultsOnly, Category = "Physics", meta = (AllowPrivateAccess = true))
	TObjectPtr<UPhysicsDataAsset> PhysicsDataAsset;
	
public:
	TObjectPtr<ASoulCharacterBase> CriticalHitTarget;
	
};
