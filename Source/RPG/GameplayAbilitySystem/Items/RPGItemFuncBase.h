// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGItemFuncBase.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGItemFuncBase : public UObject
{
	GENERATED_BODY()
	
	
	UFUNCTION(BlueprintCallable)
	virtual bool UseItem(AActor* Instigator, AActor* Target) { return false; }
};
