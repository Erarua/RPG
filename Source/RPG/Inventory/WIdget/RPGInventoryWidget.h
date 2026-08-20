// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGSlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "RPG/Inventory/Data/RPGInventorySlotData.h"
#include "RPG/Inventory/Manager/RPGInventoryComponent.h"
#include "RPGInventoryWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlotUpdated, int32, const FRPGInventorySlotData&);


/**
 * 
 */
UCLASS()
class RPG_API URPGInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	void InitializeInventoryWidget(URPGInventoryComponent* InvComp);
	
	FOnSlotUpdated OnSlotUpdatedDelegate;


protected:
	
	UPROPERTY()
	TArray<TObjectPtr<URPGSlotWidget>> SlotWidgets;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> Grid;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumColumns = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumRows = 0;
	
	TWeakObjectPtr<URPGInventoryComponent> InventoryComponent;
	
	virtual void NativeDestruct() override;
	
	
private:
	
	UFUNCTION()
	void OnSlotUpdated(int32 SlotIndex, const FRPGInventorySlotData &SlotData);
	
	UFUNCTION()
	void OnSlotSwapped(int32 SourceIndex, int32 TargetIndex);
	
	UFUNCTION()
	void OnSlotRemoved(int32 SourceIndex);
	
	UFUNCTION()
	void OnSlotUsed(int32 SourceIndex);
	
};
