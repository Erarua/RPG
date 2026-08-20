// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGDamageRule_Basic.h"

void URPGDamageRule_Basic::ApplyRule(const FRPGDamagePayload_Base& DamagePayload,
	FRPGDamagePayload_Base& OutModifiedDamagePayload) const
{
	// Start with the base damage
	float RemainingDamage = DamagePayload.BaseDamage;
	
	// Apply armor reduction
	float Armor = DamagePayload.TargetAttributes.Contains(FGameplayTag::RequestGameplayTag(FName("Data.Target.Armor"))) ? DamagePayload.TargetAttributes[FGameplayTag::RequestGameplayTag(FName("Data.Target.Armor"))] : 0.f;
	float ArmorReduction = Armor * 0.5f; // Example: each point of armor reduces damage by 0.5
	RemainingDamage = FMath::Max(RemainingDamage - ArmorReduction, 0.f);
	
	// Apply shield reduction
	float Shield = DamagePayload.TargetAttributes.Contains(FGameplayTag::RequestGameplayTag(FName("Data.Target.Shield"))) ? DamagePayload.TargetAttributes[FGameplayTag::RequestGameplayTag(FName("Data.Target.Shield"))] : 0.f;
	float ShieldAbsorption = FMath::Min(RemainingDamage, Shield);
	RemainingDamage -= ShieldAbsorption;
	
	// Set the final damage values in the output payload
	OutModifiedDamagePayload.FinalHealthDamage = RemainingDamage;
	OutModifiedDamagePayload.FinalShieldDamage = ShieldAbsorption;
}
