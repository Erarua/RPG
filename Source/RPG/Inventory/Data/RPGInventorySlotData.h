// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RPGInventorySlotData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType, Blueprintable)
struct FRPGInventorySlotData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	FName ItemID = NAME_None;
	
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 Amount = 0;
	
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	bool bIsLocked = false; 
	
	bool IsEmpty() const
	{
		return ItemID.IsNone() || Amount <= 0;
	}
	
};
