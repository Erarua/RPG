// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGDamageRule_Base.h"
#include "RPGDamageRule_EMP.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGDamageRule_EMP : public URPGDamageRule_Base
{
	GENERATED_BODY()
	
	URPGDamageRule_EMP();
	
	virtual void ApplyRule(const FRPGDamagePayload_Base& DamagePayload, FRPGDamagePayload_Base& OutModifiedDamagePayload) const override;
};
