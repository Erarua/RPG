#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RPG/GameplayAbilitySystem/Items/RPGItemBase.h"
#include "RPG/GameplayAbilitySystem/Items/RPGItemFuncBase.h"
#include "RPGItemData.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FRPGItemData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FName ItemID;
	
	UPROPERTY(EditDefaultsOnly)
	FText ItemName;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URPGItemDefinition> ItemDef;
	
	// UPROPERTY(EditDefaultsOnly)
	// int32 MaxStackSize = 1;
	
	// UPROPERTY(EditDefaultsOnly)
	// bool bNeverDecrease = false;
	
	// UPROPERTY(EditDefaultsOnly)
	// TSoftClassPtr<URPGItemFuncBase> ItemFuncClass;
	//
	// UPROPERTY(EditDefaultsOnly)
	// TSoftClassPtr<ARPGItemBase> ItemActorClass;
	
};


