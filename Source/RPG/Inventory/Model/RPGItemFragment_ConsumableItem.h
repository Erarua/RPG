// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGItemFragment.h"
#include "RPGItemFragment_ConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGItemFragment_ConsumableItem : public URPGItemFragment
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	int32 ConsumeStack = 0;
};
