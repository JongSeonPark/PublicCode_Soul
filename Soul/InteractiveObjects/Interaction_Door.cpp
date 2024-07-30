// Copyright 2024 ChickenGames All rights reserved.


#include "Interaction_Door.h"

#include "SoulGameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Game/SoulGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Save/SoulSaveGame.h"

AInteraction_Door::AInteraction_Door()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);
	TravelTransformComponent = CreateDefaultSubobject<USceneComponent>("TravelTransformComponent");
	TravelTransformComponent->SetupAttachment(BoxComponent);
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	ArrowComponent->SetupAttachment(TravelTransformComponent);
}

FTransform AInteraction_Door::GetTravelTransform() const
{
	return TravelTransformComponent->GetComponentTransform();	
}

void AInteraction_Door::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteraction_Door::Interaction_Implementation(ASoulCharacterBase* InteractionCharacter)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), Level);
	if (USoulGameInstance* SoulGameInstance = USoulGameplayStatics::GetSoulGameInstance(GetWorld()))
	{
		if (USoulSaveGame* SoulSaveGame = SoulGameInstance->SoulSaveGame)
		{
			if (!DoorKey.IsNone())
			{
				SoulSaveGame->DoorKey = DoorKey;
			}
		}
		SoulGameInstance->SaveCurrentGame();
	}
}

FText AInteraction_Door::GetInteractionInfo_Implementation()
{
	return InteractionInfo;
}

