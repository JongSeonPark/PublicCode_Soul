// Copyright 2024 ChickenGames All rights reserved.


#include "SoulArmor.h"

#include "Character/SoulCharacterBase.h"


void ASoulArmor::BeginPlay()
{
	Super::BeginPlay();
	
	if (ASoulCharacterBase* OwnerCharacter = Cast<ASoulCharacterBase>(GetOwner()))
	{
		for (FArmorMeshData ArmorMeshData : ArmorMeshDatas)
		{
			USkeletalMeshComponent* NewComp = NewObject<USkeletalMeshComponent>(this);
			NewComp->RegisterComponent();
			NewComp->SetSkeletalMesh(ArmorMeshData.Mesh);
			NewComp->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), ArmorMeshData.SocketName);
		}
	}
}