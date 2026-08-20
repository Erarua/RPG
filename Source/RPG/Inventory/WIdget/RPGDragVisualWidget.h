// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPGDragVisualWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGDragVisualWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
	class UImage* DragIcon;
	
	void SetDragIcon(class UTexture2D* InTexture);
};
