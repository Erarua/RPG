// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "RPG/Inventory/Data/RPGInventorySlotData.h"
#include "RPG/Inventory/Data/RPGSlotDragDropOp.h"
#include "RPG/Inventory/Manager/Asset/RPGInvetoryAssetManager.h"
#include "RPGDragVisualWidget.h"



void URPGSlotWidget::RefreshWidget(const FRPGInventorySlotData& SlotData)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Slot Index=%i"), *FDateTime::UtcNow().ToString(), SlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("Data: ItemID=%s, Amount=%i, Locked=%s"), *SlotData.ItemID.ToString(), SlotData.Amount, SlotData.bIsLocked ? TEXT("true") : TEXT("false"));
	bIsLocked = SlotData.bIsLocked;
	if (SlotData.bIsLocked)
	{
		ItemIcon->SetBrush(LockImg->GetBrush());
		ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	if (SlotData.IsEmpty())
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		AmountText->SetVisibility(ESlateVisibility::Hidden);
		return;
	} 
	
	FRPGItemData* ItemData = URPGInvetoryAssetManager::GetItem(SlotData.ItemID);
	
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData not found for ItemID: %s"), *SlotData.ItemID.ToString());
		return;
	}
	
	ItemIcon->SetRenderOpacity(1.0f);
	
	ItemIcon->SetBrushFromTexture(ItemData->ItemIcon.LoadSynchronous());
	ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	if (SlotData.Amount > 1)
	{
		AmountText->SetText(FText::AsNumber(SlotData.Amount));
		AmountText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		AmountText->SetVisibility(ESlateVisibility::Hidden);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[%s] Slot Index=%i Refreshed. ItemID=%s, Amount=%i, Locked=%s"), *FDateTime::UtcNow().ToString(), SlotIndex, *SlotData.ItemID.ToString(), SlotData.Amount, SlotData.bIsLocked ? TEXT("true") : TEXT("false"));
}

FReply URPGSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && !bIsLocked && ItemIcon->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && !bIsLocked && ItemIcon->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		OnItemUsed.Broadcast(SlotIndex);
		return FReply::Handled();
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void URPGSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	URPGSlotDragDropOp *DragDropOp = NewObject<URPGSlotDragDropOp>();
	DragDropOp->SourceSlotIndex = SlotIndex;
	
	// 2. 生成跟随鼠标的替身图标
	if (DragVisualClass)
	{
		URPGDragVisualWidget* DragVisual = CreateWidget<URPGDragVisualWidget>(this, DragVisualClass);
		if (DragVisual)
		{
			// 从当前格子的 ItemIcon 中提取出当前的贴图，传给替身
			if (UTexture2D* CurrentTex = Cast<UTexture2D>(ItemIcon->GetBrush().GetResourceObject()))
			{
				DragVisual->SetDragIcon(CurrentTex);
			}

			// 赋值给底层的默认拖拽视觉外观，引擎会自动让它跟着鼠标走
			DragDropOp->DefaultDragVisual = DragVisual;
			// 设置鼠标抓取替身的位置（居中）
			DragDropOp->Pivot = EDragPivot::CenterCenter;
		}
	}

	// 3. 极佳的交互细节：把被拖起的原格子变暗（或者半透明）
	ItemIcon->SetRenderOpacity(0.3f);
	
	// 4. 绑定拖拽取消事件（如果玩家把物品拖到了 UI 外面松开）
	DragDropOp->OnDragCancelled.AddDynamic(this, &URPGSlotWidget::MyOnDragCancelled);
	DragDropOp->OnDrop.AddDynamic(this, &URPGSlotWidget::MyOnDragSuccess);
	
	OutOperation = DragDropOp;
}

bool URPGSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (URPGSlotDragDropOp* DragDropOp = Cast<URPGSlotDragDropOp>(InOperation))
	{
		if (DragDropOp->SourceSlotIndex != SlotIndex)
		{
			UE_LOG(LogTemp, Warning, TEXT("Drop On Slot %d"), SlotIndex);
			OnSlotSwapped.Broadcast(DragDropOp->SourceSlotIndex, SlotIndex);
		}
		return true;
	}
	
	return false;
}

void URPGSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	if (HighlightImg)
	{
		HighlightImg->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void URPGSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	if (HighlightImg)
	{
		HighlightImg->SetVisibility(ESlateVisibility::Hidden);
	}
}

void URPGSlotWidget::MyOnDragCancelled(UDragDropOperation* InOperation)
{
	if (URPGSlotDragDropOp* DragDropOp = Cast<URPGSlotDragDropOp>(InOperation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Drag cancelled for slot index: %d"), DragDropOp->SourceSlotIndex);
		OnItemRemoved.Broadcast(DragDropOp->SourceSlotIndex);
	}

	ItemIcon->SetRenderOpacity(1.0f);
}

void URPGSlotWidget::MyOnDragSuccess(UDragDropOperation* InOperation)
{
	ItemIcon->SetRenderOpacity(1.0f);
}
