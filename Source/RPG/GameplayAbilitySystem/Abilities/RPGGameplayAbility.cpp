// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGGameplayAbility.h"

URPGGameplayAbility::URPGGameplayAbility()
{
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Active")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
}
