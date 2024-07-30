// Copyright 2024 ChickenGames All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObjects/InteractiveObjectInterface.h"
#include "Item/SoulItemTypes.h"
#include "SoulWorldItem.generated.h"

UCLASS()
class SOUL_API ASoulWorldItem : public AActor, public IInteractiveObjectInterface
{
	GENERATED_BODY()

public:
	ASoulWorldItem();
	virtual void Interaction_Implementation(ASoulCharacterBase* InteractionCharacter) override;
	virtual FText GetInteractionInfo_Implementation() override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FSoulItemData> Items;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText InteractionInfo;
	
};
