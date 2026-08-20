// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGDamageRule_EMP.h"

URPGDamageRule_EMP::URPGDamageRule_EMP()
{
	RuleTag = FGameplayTag::RequestGameplayTag(FName("DamageRule.EMP"));
}

void URPGDamageRule_EMP::ApplyRule(const FRPGDamagePayload_Base& DamagePayload,
                                   FRPGDamagePayload_Base& OutModifiedDamagePayload) const
{
	OutModifiedDamagePayload.FinalShieldDamage = DamagePayload.BaseShieldDamage;
	OutModifiedDamagePayload.FinalHealthDamage = 0.f;
}
