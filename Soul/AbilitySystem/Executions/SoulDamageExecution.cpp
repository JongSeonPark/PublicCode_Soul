// Copyright 2024 ChickenGames All rights reserved.


#include "AbilitySystem/Executions/SoulDamageExecution.h"

#include "SoulGameplayTags.h"
#include "AbilitySystem/AttributeSet/SoulAttributeSet.h"
#include "Item/Equipments/SoulShield.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BasePhysicalPowerDef;

	FGameplayEffectAttributeCaptureDefinition BasePhysicalDefenseDef;
	FGameplayEffectAttributeCaptureDefinition BasePoiseDef;
	FGameplayEffectAttributeCaptureDefinition BaseStaminaPowerDef;

	FDamageStatics()
	{
		BasePhysicalPowerDef = FGameplayEffectAttributeCaptureDefinition(USoulAttributeSet::GetPhysicalPowerAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);

		BasePhysicalDefenseDef = FGameplayEffectAttributeCaptureDefinition(USoulAttributeSet::GetPhysicalDefenseAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
		BasePoiseDef = FGameplayEffectAttributeCaptureDefinition(USoulAttributeSet::GetPoiseAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
		BaseStaminaPowerDef = FGameplayEffectAttributeCaptureDefinition(USoulAttributeSet::GetStaminaAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

USoulDamageExecution::USoulDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().BasePhysicalPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().BasePhysicalDefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().BasePoiseDef);
	RelevantAttributesToCapture.Add(DamageStatics().BaseStaminaPowerDef);
}

void USoulDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContext* Context = Spec.GetContext().Get();
	TWeakObjectPtr<AActor> TargetActor = Context->GetActors().Num() > 0 ? Context->GetActors()[0] : nullptr;
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// TODO: 장비 아이템에 관하여 정리되면 추가 작업.
	bool bIsGuard = TargetTags->HasTagExact(SoulGameplayTags::Status_Guard) && TargetActor.IsValid() && TargetActor.Get()->IsA<ASoulShield>();
	if (bIsGuard)
	{
		ExecuteGuardDamage(ExecutionParams, OutExecutionOutput);
	}
	else
	{
		ExecuteDamage(ExecutionParams, OutExecutionOutput);
	}
}

void USoulDamageExecution::ExecuteDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// TODO: Magic Damage 설정
	float SourcePhyscialPower = 1.f;
	float TargetPhyscialDefence = 1.f;
	float TargetPoise = 1.f;
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BasePhysicalPowerDef, EvaluationParameters, SourcePhyscialPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BasePhysicalDefenseDef, EvaluationParameters, TargetPhyscialDefence);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BasePoiseDef, EvaluationParameters, TargetPoise);

	// 슈퍼아머 강인도=(갑옷 강인도)+(무기별 강인도 보정)x(슈퍼 아머 배율)-0.5x(갑옷 강인도)
	// TODO: 여기서는 Poise값을 일정값 올려주고 끝내겠음. 추후 변경.
	bool bIsHyperArmor = TargetTags->HasTagExact(SoulGameplayTags::Status_HyperArmor);
	if (bIsHyperArmor)
		TargetPoise += 0.4f;

	TargetPoise = 1.f - FMath::Clamp(TargetPoise, 0.f, 0.8f);

	float Damage = Spec.GetSetByCallerMagnitude(SoulGameplayTags::SetByCaller_Damage);
	float StanceDamage = Spec.GetSetByCallerMagnitude(SoulGameplayTags::SetByCaller_Damage_Stance);
	float PoiseDamage = Spec.GetSetByCallerMagnitude(SoulGameplayTags::SetByCaller_Damage_Poise);

	// TODO: 정확한 방어력 계산
	Damage = CalculateDamage(Damage * SourcePhyscialPower, TargetPhyscialDefence);

	StanceDamage = FMath::Max(0, StanceDamage * TargetPoise);
	PoiseDamage = FMath::Max(0, PoiseDamage * TargetPoise);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USoulAttributeSet::GetDamageAttribute(), EGameplayModOp::Override, Damage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USoulAttributeSet::GetStanceDamageAttribute(), EGameplayModOp::Override, StanceDamage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USoulAttributeSet::GetPoiseDamageAttribute(), EGameplayModOp::Override, PoiseDamage));
}

void USoulDamageExecution::ExecuteGuardDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// TODO: Magic Damage 설정
	float SourcePhyscialPower = 0.f;
	float TargetPhyscialDefence = 0.f;
	float TargetCurrentStamina = 0.f;
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BasePhysicalPowerDef, EvaluationParameters, SourcePhyscialPower);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BasePhysicalDefenseDef, EvaluationParameters, TargetPhyscialDefence);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseStaminaPowerDef, EvaluationParameters, TargetCurrentStamina);

	float Damage = Spec.GetSetByCallerMagnitude(SoulGameplayTags::SetByCaller_Damage);
	float StaminaReduce = 50.f;

	if (StaminaReduce > TargetCurrentStamina)
	{
		UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
		TargetASC->TryActivateAbilitiesByTag(FGameplayTagContainer(SoulGameplayTags::Status_GuardBreak));
	}

	// Damage = FMath::Max(1, Damage * SourcePhyscialPower / TargetPhyscialDefence) / 5.f;
	Damage = CalculateDamage(Damage * SourcePhyscialPower, TargetPhyscialDefence);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USoulAttributeSet::GetDamageAttribute(), EGameplayModOp::Override, Damage));
	// TODO: 가드 강도에 따라 스테미너 계산
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(USoulAttributeSet::GetStaminaAttribute(), EGameplayModOp::Additive, -StaminaReduce));
}

float USoulDamageExecution::CalculateDamage(float Damage, float Defence) const
{
	if (Defence > Damage * 8.f) return Damage * 0.1f;
	if (Defence > Damage) return (19.2f / 49.f * FMath::Pow(Damage / Defence - 0.125f, 2) + 0.1f) * Damage;
	if (Defence > Damage * 0.4f) return (-0.4f / 3.f * FMath::Pow(Damage / Defence - 0.25f, 2) + 0.7f) * Damage;
	if (Defence > Damage * 0.125f) return (-0.8f / 121.f * FMath::Pow(Damage / Defence - 8.f, 2) + 0.9f) * Damage;
	return Damage * 0.9f;
}
