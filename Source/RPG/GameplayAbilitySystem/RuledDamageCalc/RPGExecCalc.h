// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RPGExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	URPGExecCalc();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
