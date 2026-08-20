// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "RPGEffectComp_Base.generated.h"

class URPGDamageRule_Base;
/**
 * 
 */
UCLASS()
class RPG_API URPGEffectComp_Base : public UGameplayEffectComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<URPGDamageRule_Base>> DamageRules;
};
