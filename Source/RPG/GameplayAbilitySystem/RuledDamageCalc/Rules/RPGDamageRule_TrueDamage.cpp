// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGDamageRule_TrueDamage.h"

URPGDamageRule_TrueDamage::URPGDamageRule_TrueDamage()
{
	RuleTag = FGameplayTag::RequestGameplayTag(FName("DamageRule.TrueDamage"));
}

void URPGDamageRule_TrueDamage::ApplyRule(const FRPGDamagePayload_Base& DamagePayload,
                                          FRPGDamagePayload_Base& OutModifiedDamagePayload) const
{
	OutModifiedDamagePayload.FinalHealthDamage = DamagePayload.BaseHealthDamage;
}
