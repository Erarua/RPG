// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"

#include "RPGPlayerState.h"
#include "RPG/Inventory/Manager/UI/RPGInventoryUIManager.h"

void ARPGPlayerController::ToggleInventory()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (URPGInventoryUIManager* UIManager = LocalPlayer->GetSubsystem<URPGInventoryUIManager>())
		{
			UIManager->ToggleInventoryUI();
		}
	}
}

void ARPGPlayerController::AddRandomItemToInventory()
{
	if (ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>())
	{
		if (URPGInventoryComponent* InvComp = PS->FindComponentByClass<URPGInventoryComponent>())
		{
			InvComp->Server_AddRandomItem();
		}
	}
}
