// Copyright 2024 ChickenGames All rights reserved.


#include "AnimNotify_FootStep.h"

#include "NiagaraFunctionLibrary.h"
#include "Soul.h"
#include "Character/SoulCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Physics/PhysicsDataAsset.h"

void UAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	const UWorld* World = MeshComp->GetWorld();
	const ISoulPhysicsInterface* PhysicsInterface = Cast<ISoulPhysicsInterface>(MeshComp->GetOwner());  
	if (!World || !PhysicsInterface) return;
	const FTransform FootTranform = MeshComp->GetBoneTransform(FootBoneName);
	if (!FootTranform.IsValid())
	{
		SOUL_LOG(Warning, TEXT("%s에 해당하는 본이 없음."), *FootBoneName.ToString());
		return;
	}

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(
		World,
		FootTranform.GetLocation(),
		FootTranform.GetLocation() - FVector(0, 0, TraceLength),
		TraceTypeQuery1, //Visible
		false,
		TArray<AActor*>(),
		bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
		);

	if (HitResult.PhysMaterial.IsValid())
	{
		FPhysicsEffectData EffectData;
		if (PhysicsInterface->TryGetPhysicsEffectData(HitResult.PhysMaterial->SurfaceType, EffectData))
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				World,
				EffectData.NiagaraEffect,
				HitResult.ImpactPoint
				);
			UGameplayStatics::PlaySoundAtLocation(
				World,
				EffectData.SoundEffect,
				HitResult.ImpactPoint
				);
		}
	}
}
