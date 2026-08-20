// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPG/Inventory/Manager/RPGInventoryComponent.h"
#include "RPGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	
	ARPGPlayerState();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<URPGInventoryComponent> InventoryComponent;
	
};
