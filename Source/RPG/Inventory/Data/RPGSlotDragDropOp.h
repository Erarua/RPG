// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "RPGSlotDragDropOp.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGSlotDragDropOp : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	int32 SourceSlotIndex = -1;
};
