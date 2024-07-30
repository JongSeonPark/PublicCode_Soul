// Copyright 2024 ChickenGames All rights reserved.


#include "Animation/AnimNotifies/AnimNotifyState_Attack.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Item/Equipments/SoulWeapon.h"
#include "Character/SoulCharacterBase.h"
#include "Inventory/SoulInventoryManagerComponent.h"

void UAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (ASoulWeapon* Weapon = GetWeapon(MeshComp))
	{
		Weapon->BeginAttack(DamageRate, StanceDamageRate, PoiseDamageRate);
		if (UFXSystemComponent* Component = SpawnEffect(Weapon, Animation))
		{
			Component->ComponentTags.AddUnique(GetSpawnedComponentTag());
		}
	}
}

void UAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (ASoulWeapon* Weapon = GetWeapon(MeshComp))
	{
		Weapon->EndAttack();
		if (UFXSystemComponent* FXComponent = GetSpawnedEffect(Weapon))
		{
			// untag the component
			FXComponent->ComponentTags.Remove(GetSpawnedComponentTag());
			FXComponent->DestroyComponent();
		}
	}
}

UFXSystemComponent* UAnimNotifyState_Attack::GetSpawnedEffect(ASoulWeapon* Weapon)
{
	TArray<USceneComponent*> Children;
	Weapon->GetMesh()->GetChildrenComponents(false, Children);

	if (Children.Num())
	{
		for (USceneComponent* Component : Children)
		{
			if (Component && Component->ComponentHasTag(GetSpawnedComponentTag()))
			{
				if (UFXSystemComponent* FXComponent = CastChecked<UFXSystemComponent>(Component))
				{
					return FXComponent;
				}
			}
		}
	}

	return nullptr;
}

UFXSystemComponent* UAnimNotifyState_Attack::SpawnEffect(ASoulWeapon* Weapon, UAnimSequenceBase* Animation) const
{
	if (ValidateParameters(Weapon))
	{
		if (UNiagaraComponent* NewComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(Trail, Weapon->GetMesh(), AttachSocketName, FVector::Zero(), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true))
		{
			return NewComponent;
		}
	}
	return nullptr;
}

bool UAnimNotifyState_Attack::ValidateParameters(ASoulWeapon* Weapon) const
{
	auto MeshComp = Weapon->GetMesh();
	bool bValid = true;
	if (!Trail)
	{
		bValid = false;
	}
	else if (!MeshComp->DoesSocketExist(AttachSocketName) && MeshComp->GetBoneIndex(AttachSocketName) == INDEX_NONE)
	{
		bValid = false;
	}
	
	return bValid;
}

ASoulWeapon* UAnimNotifyState_Attack::GetWeapon(USkeletalMeshComponent* MeshComp) const
{
	if (const ASoulCharacterBase* Character = Cast<ASoulCharacterBase>(MeshComp->GetOwner()))
	{
		if (ASoulEquipment* Equip = Character->GetInventoryManagerComponent()->GetEquipmentItem(WeaponSlotType))
		{
			return Cast<ASoulWeapon>(Equip);
		}
	}
	
	return nullptr;
}
