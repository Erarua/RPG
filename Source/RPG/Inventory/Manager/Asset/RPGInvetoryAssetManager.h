// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RPG/Inventory/Data/RPGItemData.h"
#include "RPGInvetoryAssetManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_API URPGInvetoryAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static URPGInvetoryAssetManager& Get()
	{
		check(GEngine);
    
		URPGInvetoryAssetManager* Singleton = Cast<URPGInvetoryAssetManager>(GEngine->AssetManager);
		if (!Singleton)
		{
			UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager! Please check your DefaultEngine.ini or Project Settings."));
		}

		return *Singleton;
	}
	
	static FRPGItemData* GetItem(FName ItemID)
	{
		
		if (UDataTable* OutItemTable = Get().GetAssetSync<UDataTable>(Get().DTItem))
		{
			FRPGItemData* ItemData = OutItemTable->FindRow<FRPGItemData>(ItemID, TEXT("GetItemTable"));
			checkf(ItemData, TEXT("ItemID %s not found in ItemTable!"), *ItemID.ToString());
			return ItemData;
		}
		
		return nullptr;
	}
	
	template<typename AssetClass>
	AssetClass* GetAssetSync(const TSoftObjectPtr<AssetClass>& SoftPtr)
	{
		AssetClass* Asset = SoftPtr.LoadSynchronous();
		return Asset;
	}
	
	const UDataTable* GetItemTable()
	{
		if (UDataTable* OutItemTable = GetAssetSync<UDataTable>(DTItem))
		{
			return OutItemTable;
		}
		else
		{
			UE_LOG(LogTemp, Fatal, TEXT("ItemTable not found in AssetManager!"));
			return nullptr;
		}
	}
	
	virtual void StartInitialLoading() override
	{
		Super::StartInitialLoading();
		
		if (!DTItem.IsNull())
		{
			ItemTable = GetAssetSync<UDataTable>(DTItem);
		}
	}
	
	UPROPERTY(EditDefaultsOnly, Category = "DT")
	TSoftObjectPtr<UDataTable> DTItem;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<class URPGInventoryWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSoftClassPtr<class URPGSlotWidget> ItemWidgetClass;
	
private:
	
	UPROPERTY()
	TObjectPtr<UDataTable> ItemTable;
	
};
