// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "RPGItemFragment.h"
#include "RPGItemFragment_StackableItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGItemFragment_StackableItem : public URPGItemFragment
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxStack = 1;
	
};
