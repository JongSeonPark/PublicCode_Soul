// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SoulCharacterBase.h"
#include "SoulPlayerCharacter.generated.h"

class USoulSaveGame;
class UBoxComponent;
class UGameplayAbility;
class UInputMappingContext;
class UInputAction;
class UPlayerLockOnComponent;
class USoulAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SOUL_API ASoulPlayerCharacter : public ASoulCharacterBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	ASoulPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetInteractionActorsInArea() const { return InteractionActorsInArea; }

protected:
	virtual void InitializeAbilitySystem() override;
	virtual void ApplyStartupEffectsToSelf() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LockOn")
	TObjectPtr<UPlayerLockOnComponent> LockOnComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UBoxComponent> InteractionAreaComponent;
private:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LockOn();

	UFUNCTION()
	void OnInteractionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnInteractionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnSaveGame(USoulSaveGame* SaveGame);

	TArray<AActor*> InteractionActorsInArea;
	
#pragma region InputAction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnAction;
#pragma endregion

};
