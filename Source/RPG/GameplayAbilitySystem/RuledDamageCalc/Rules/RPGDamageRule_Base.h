// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPG/GameplayAbilitySystem/RuledDamageCalc/DamageData/RPGDamagePayload_Base.h"
#include "UObject/Object.h"
#include "RPGDamageRule_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGDamageRule_Base : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag RuleTag;
	
	UFUNCTION(BlueprintCallable)
	virtual void ApplyRule(const FRPGDamagePayload_Base& DamagePayload, FRPGDamagePayload_Base& OutModifiedDamagePayload) const PURE_VIRTUAL();
	
	
private:
	const FGameplayTagContainer RequiredTargetAttributeTags;
	const FGameplayTagContainer RequiredSourceAttributeTags;
};
