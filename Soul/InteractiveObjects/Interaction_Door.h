// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveObjectInterface.h"
#include "GameFramework/Actor.h"
#include "Interaction_Door.generated.h"

class UArrowComponent;
class UBoxComponent;

/**
 * 레벨 이동용 문
 */
UCLASS()
class SOUL_API AInteraction_Door : public AActor, public IInteractiveObjectInterface
{
	GENERATED_BODY()

public:
	AInteraction_Door();
	FName GetDoorKey() const { return DoorKey; }
	FTransform GetTravelTransform() const;

protected:
	virtual void BeginPlay() override;
	virtual void Interaction_Implementation(ASoulCharacterBase* InteractionCharacter) override;
	virtual FText GetInteractionInfo_Implementation() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> TravelTransformComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> ArrowComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName DoorKey;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> Level;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FText InteractionInfo;
};
