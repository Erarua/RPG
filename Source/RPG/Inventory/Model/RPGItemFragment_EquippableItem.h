// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGGESetting.h"
#include "RPGItemFragment.h"
#include "Abilities/GameplayAbility.h"
#include "RPGItemFragment_EquippableItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGItemFragment_EquippableItem : public URPGItemFragment
{
	GENERATED_BODY()
	
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> ActiveAbilityClass;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilityClasses;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<URPGGESetting>> PassiveEffectSetting;
	
};
