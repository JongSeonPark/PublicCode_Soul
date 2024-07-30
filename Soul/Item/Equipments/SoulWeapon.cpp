// Copyright 2024 ChickenGames All rights reserved.


#include "SoulWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Soul.h"
#include "Character/SoulEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemComponent.h"
#include "EquipmentDatas.h"
#include "NiagaraFunctionLibrary.h"
#include "SoulGameplayTags.h"
#include "SoulShield.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Physics/PhysicsDataAsset.h"

ASoulWeapon::ASoulWeapon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(Mesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	BoxComponent->SetCollisionProfileName(TEXT("Weapon"));
	BoxComponent->SetupAttachment(Mesh);
}

void ASoulWeapon::BeginPlay()
{
	Super::BeginPlay();

	// 공격 중일때만 허용
	BoxComponent->SetGenerateOverlapEvents(false);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASoulWeapon::OnBeginOverlap);

	if (ASoulCharacterBase* OwnerCharacter = Cast<ASoulCharacterBase>(GetOwner()))
	{
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), FName("hand_weapon_r"));
		if (EquipmentAbilityDataAsset->AnimClassLayer)
		{
			OwnerCharacter->GetMesh()->LinkAnimClassLayers(EquipmentAbilityDataAsset->AnimClassLayer);
		}
	}
}

void ASoulWeapon::Destroyed()
{
	Super::Destroyed();
	if (ASoulCharacterBase* OwnerCharacter = Cast<ASoulCharacterBase>(GetOwner()))
	{
		if (EquipmentAbilityDataAsset->AnimClassLayer)
		{
			OwnerCharacter->GetMesh()->UnlinkAnimClassLayers(EquipmentAbilityDataAsset->AnimClassLayer);
		}
	}
}

void ASoulWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!AbilitySystemComponent) return;
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Other);
	if (AttackedEnemyASCs.Contains(TargetASC)) return;
	if (!TargetASC || TargetASC == AbilitySystemComponent) return;
	if (TargetASC->HasAnyMatchingGameplayTags(SoulGameplayTags::InvincibleTags)) return;

	// TODO: 팀이 정립되면 적을 구분.
	if ((Other->IsA<ASoulCharacterBase>() || Other->IsA<ASoulShield>()) && TargetASC != AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		// OnComponentBeginOverlap이 루트 컴포넌트였을 때만 SweepResult에 Location등의 결과가 채워지기에 한번 더 체크.
		if (bFromSweep)
		{
			EffectContextHandle.AddHitResult(SweepResult);
		}
		else
		{
			TArray<FHitResult> HitResults;
			BoxTrace(HitResults);
			if (FHitResult* HitResult = HitResults.FindByPredicate([Other](FHitResult Hit) { return Hit.GetActor() == Other; }))
			{
				EffectContextHandle.AddHitResult(*HitResult);
			}
		}

		if (AbilitySystemComponent->IsOwnerActorAuthoritative())
		{
			const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AttackEffectClass, 0, EffectContextHandle);
			EffectContextHandle.AddActors({Other});
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage, DamageRate);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage_Stance, StanceDamageRate);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage_Poise, PoiseDamageRate);
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetASC);
		}
		
		AttackedEnemyASCs.Add(TargetASC);
	}

	// SOUL_LOG(Display, TEXT("Overlap! %s"), *Other->GetName());
}

void ASoulWeapon::BeginAttack(float InDamageRate, float InStanceDamageRate, float InPoiseDamageRate)
{
	BoxComponent->SetGenerateOverlapEvents(true);
	DamageRate = InDamageRate;
	StanceDamageRate = InStanceDamageRate;
	PoiseDamageRate = InPoiseDamageRate;
	GetWorldTimerManager().SetTimer(WeaponScrachTimerHandle, this, &ASoulWeapon::OnScrachUpdate, 0.025f, true);
	CheckBlockedByWall();
} 

void ASoulWeapon::EndAttack()
{
	BoxComponent->SetGenerateOverlapEvents(false);
	GetWorldTimerManager().ClearTimer(WeaponScrachTimerHandle);
	AttackedEnemyASCs.Empty();
}

void ASoulWeapon::CheckBlockedByWall()
{
	TArray<FHitResult> HitResults;
	BoxTrace(HitResults);
	for (FHitResult HitResult : HitResults)
	{
		if (ECC_WorldStatic == HitResult.Component.Get()->BodyInstance.GetObjectType())
		{
			AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(SoulGameplayTags::Cancel_Attack_WallImpact));
			TryActPhysicsEffect(HitResults);
			break;
		}
	}
}

void ASoulWeapon::OnScrachUpdate()
{
	TArray<FHitResult> HitResults;
	BoxTrace(HitResults);
	TryActPhysicsEffect(HitResults);
}

FGameplayEffectSpecHandle ASoulWeapon::MakeDamageEffectSpec(const TSubclassOf<UGameplayEffect>& EffectClass, const FHitResult& SweepResult) const
{
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddHitResult(SweepResult);
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 0, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage, DamageRate);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage_Stance, StanceDamageRate);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, SoulGameplayTags::SetByCaller_Damage_Poise, PoiseDamageRate);
	return EffectSpecHandle;
}

bool ASoulWeapon::BoxTrace(TArray<FHitResult>& HitResults)
{
	const bool bSuccess = UKismetSystemLibrary::BoxTraceMultiByProfile(
		this,
		BoxComponent->GetComponentLocation(),
		BoxComponent->GetComponentLocation(),
		BoxComponent->GetScaledBoxExtent(),
		BoxComponent->GetComponentRotation(),
		TEXT("Weapon"),
		false,
		TArray<AActor*>({ this, GetOwner() }),
		bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		false
	);
	return bSuccess;
}

void ASoulWeapon::TryActPhysicsEffect(const TArray<FHitResult>& HitResults) const
{
	TSet<AActor*> HitActors;
	TSet<TWeakObjectPtr<UPhysicalMaterial>> Physics;
	for (const FHitResult& HitResult : HitResults)
	{
		if (Physics.Contains(HitResult.PhysMaterial)) continue;;
		if (HitActors.Contains(HitResult.GetActor())) continue;
		Physics.Add(HitResult.PhysMaterial);
		HitActors.Add(HitResult.GetActor());
		TryActPhysicsEffect(HitResult);
	}
}

void ASoulWeapon::TryActPhysicsEffect(const FHitResult& HitResult) const
{
	if (HitResult.PhysMaterial.IsValid() &&
		HitResult.GetActor() != AbilitySystemComponent->GetAvatarActor())
	{
		FPhysicsEffectData EffectData;
		if (TryGetPhysicsEffectData(HitResult.PhysMaterial->SurfaceType, EffectData))
		{
			FRotator Rot = HitResult.ImpactNormal.Rotation();
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				EffectData.NiagaraEffect,
				HitResult.ImpactPoint,
				Rot
			);
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				EffectData.SoundEffect,
				HitResult.ImpactPoint
			);
		}
	}
}
