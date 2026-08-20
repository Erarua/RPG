// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "UObject/Object.h"
#include "RPGGESetting.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI, DefaultToInstanced, EditInlineNew) 
class URPGGESetting : public UObject
{
	GENERATED_BODY()
	
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, float> SetByCallerAttributes;
	
};
