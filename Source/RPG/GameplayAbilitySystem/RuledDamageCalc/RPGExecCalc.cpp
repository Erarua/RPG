// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGExecCalc.h"

#include "GameplayEffectExtension.h"
#include "RPGEffectComp_Base.h"
#include "DamageData/RPGDamagePayload_Base.h"
#include "RPG/GameplayAbilitySystem/AttributeSets/BasicAttributeSet.h"
#include "RPG/GameplayAbilitySystem/AttributeSets/CombatAttributeSet.h"
#include "Rules/RPGDamageRule_Base.h"

namespace
{
	struct FRPGExecCalcCaptureDefs
	{
		DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
		DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
		DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

		FRPGExecCalcCaptureDefs()
		{
			// Source attack stat: snapshot at spec creation time.
			DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, Strength, Source, true);
			// Target defense stats: read current value at execution time.
			DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, Armor, Target, false);
			DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet, Shield, Target, false);
		}
	};

	struct FRPGTaggedCapture
	{
		FGameplayTag DataTag;
		const FGameplayEffectAttributeCaptureDefinition* CaptureDef = nullptr;
		bool bSource = false;
	};

	const FRPGExecCalcCaptureDefs& GetCaptureDefs()
	{
		static FRPGExecCalcCaptureDefs CaptureDefs;
		return CaptureDefs;
	}

	const TArray<FRPGTaggedCapture>& GetTaggedCaptureMap()
	{
		static const TArray<FRPGTaggedCapture> TaggedCaptures =
		{
			{ FGameplayTag::RequestGameplayTag(FName("Data.Source.Strength")), &GetCaptureDefs().StrengthDef, true },
			{ FGameplayTag::RequestGameplayTag(FName("Data.Target.Armor")), &GetCaptureDefs().ArmorDef, false },
			{ FGameplayTag::RequestGameplayTag(FName("Data.Target.Shield")), &GetCaptureDefs().ShieldDef, false },
		};
		return TaggedCaptures;
	}
}

URPGExecCalc::URPGExecCalc()
{
	RelevantAttributesToCapture.Add(GetCaptureDefs().StrengthDef);
	RelevantAttributesToCapture.Add(GetCaptureDefs().ArmorDef);
	RelevantAttributesToCapture.Add(GetCaptureDefs().ShieldDef);
}

void URPGExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	if (!Spec.Def)
	{
		return;
	}

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FRPGDamagePayload_Base DamagePayload;
	DamagePayload.BaseDamage = Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, 0.f);
	DamagePayload.BaseHealthDamage = Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.HealthDamage")), false, DamagePayload.BaseDamage);
	DamagePayload.BaseShieldDamage = Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.ShieldDamage")), false, 0.f);

	// Default flow: if no rule applies, base damage goes to health damage directly.
	DamagePayload.FinalHealthDamage = DamagePayload.BaseHealthDamage;
	DamagePayload.FinalShieldDamage = DamagePayload.BaseShieldDamage;

	for (const FRPGTaggedCapture& Entry : GetTaggedCaptureMap())
	{
		if (!Entry.CaptureDef)
		{
			continue;
		}

		float Value = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(*Entry.CaptureDef, EvalParams, Value);
		Value = FMath::Max(0.f, Value);

		if (Entry.bSource)
		{
			DamagePayload.SourceAttributes.Add(Entry.DataTag, Value);
		}
		else
		{
			DamagePayload.TargetAttributes.Add(Entry.DataTag, Value);
		}
	}

	if (const URPGEffectComp_Base* RuleComp = Spec.Def->FindComponent<URPGEffectComp_Base>())
	{
		for (const TSubclassOf<URPGDamageRule_Base>& RuleClass : RuleComp->DamageRules)
		{
			if (!RuleClass)
			{
				continue;
			}

			const URPGDamageRule_Base* RuleCDO = GetDefault<URPGDamageRule_Base>(RuleClass);
			if (!RuleCDO)
			{
				continue;
			}

			const bool bRuleHasTag = RuleCDO->RuleTag.IsValid();
			const bool bTagMatch = !bRuleHasTag || (EvalParams.SourceTags && EvalParams.SourceTags->HasTag(RuleCDO->RuleTag));
			if (!bTagMatch)
			{
				continue;
			}

			FRPGDamagePayload_Base ModifiedDamagePayload = DamagePayload;
			RuleCDO->ApplyRule(DamagePayload, ModifiedDamagePayload);

			// Advance pipeline state so the next rule consumes the previous rule output.
			DamagePayload = ModifiedDamagePayload;
			DamagePayload.BaseHealthDamage = DamagePayload.FinalHealthDamage;
			DamagePayload.BaseShieldDamage = DamagePayload.FinalShieldDamage;
		}
	}

	if (DamagePayload.FinalHealthDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UBasicAttributeSet::GetHealthDamageAttribute(),
			EGameplayModOp::Additive,
			DamagePayload.FinalHealthDamage));
	}
	
	if (DamagePayload.FinalShieldDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UBasicAttributeSet::GetShieldDamageAttribute(),
			EGameplayModOp::Additive,
			DamagePayload.FinalShieldDamage
			));
	}
}
