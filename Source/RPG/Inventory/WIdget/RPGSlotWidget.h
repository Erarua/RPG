// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGSlotWidget.generated.h"

class UUniformGridPanel;
class UImage;
class UTextBlock;
struct FRPGInventorySlotData;
class URPGInventoryWidget;
/**
 * 
 */


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlotSwapped, int32, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUsed, int32);


UCLASS()
class RPG_API URPGSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	
	UFUNCTION(BlueprintCallable)
	void SetSlotIndex(int32 Index)
	{
		SlotIndex = Index;
	}

	
	
private:
	
	UPROPERTY()
	int32 SlotIndex = -1;
	
	UPROPERTY()
	bool bIsLocked;
	
public:
	
	UFUNCTION()
	void RefreshWidget(const FRPGInventorySlotData& SlotData);
	
	
protected:
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION()
	void MyOnDragCancelled(UDragDropOperation* InOperation);
	
	UFUNCTION()
	void MyOnDragSuccess(UDragDropOperation* InOperation);
	
	
public:
	
	FOnSlotSwapped OnSlotSwapped;
	
	FOnItemRemoved OnItemRemoved;
	
	FOnItemUsed OnItemUsed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> LockImg;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> AmountText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> BackgroundImg;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	const TObjectPtr<UImage> HighlightImg;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class URPGDragVisualWidget> DragVisualClass;
	
};
