// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGInventoryWidget.h"

#include "RPG/Inventory/Manager/Asset/RPGInvetoryAssetManager.h"

void URPGInventoryWidget::InitializeInventoryWidget(URPGInventoryComponent* InvComp)
{
	InventoryComponent = InvComp;
	NumColumns = InventoryComponent->GetNumColumns();
	NumRows = InventoryComponent->GetNumRows();
	InventoryComponent->OnSlotDataUpdated.AddUObject(this, &URPGInventoryWidget::OnSlotUpdated);

	Grid->ClearChildren();
	SlotWidgets.Empty();

	int32 MaxCapacity = InventoryComponent->GetMaxCapacity();
	for (int32 i = 0; i < MaxCapacity; ++i)
	{
		if (UClass* SlotClass = URPGInvetoryAssetManager::Get().ItemWidgetClass.LoadSynchronous())
		{
			if (URPGSlotWidget* SlotWidget = CreateWidget<URPGSlotWidget>(this, SlotClass))
			{
				SlotWidget->SetSlotIndex(i);
				SlotWidget->OnSlotSwapped.AddUObject(this, &URPGInventoryWidget::OnSlotSwapped);
				SlotWidget->OnItemRemoved.AddUObject(this, &URPGInventoryWidget::OnSlotRemoved);
				SlotWidget->OnItemUsed.AddUObject(this, &URPGInventoryWidget::OnSlotUsed);
				Grid->AddChildToUniformGrid(SlotWidget, i / NumColumns, i % NumColumns);
				SlotWidgets.Add(SlotWidget);
				SlotWidget->RefreshWidget(InventoryComponent->GetSlotData(i));
			}
		}
	}
}

void URPGInventoryWidget::NativeDestruct()
{
	if (InventoryComponent.IsValid())
	{
		InventoryComponent->OnSlotDataUpdated.RemoveAll(this);
	}

	Super::NativeDestruct();
}

void URPGInventoryWidget::OnSlotUpdated(int32 SlotIndex, const FRPGInventorySlotData& SlotData)
{
	if (SlotIndex < 0 || SlotIndex >= NumColumns * NumRows) return;

	SlotWidgets[SlotIndex]->RefreshWidget(SlotData);
}

void URPGInventoryWidget::OnSlotSwapped(int32 SourceIndex, int32 TargetIndex)
{
	InventoryComponent->Server_SwapSlots(SourceIndex, TargetIndex);
}

void URPGInventoryWidget::OnSlotRemoved(int32 SourceIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget received removed slot"));
	InventoryComponent->Server_RemoveItem(SourceIndex);
}

void URPGInventoryWidget::OnSlotUsed(int32 SourceIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("InventoryWidget received used slot"));
	InventoryComponent->Server_UseItem(SourceIndex);
}
