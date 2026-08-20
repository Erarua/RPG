// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGDragVisualWidget.h"

#include "Components/Image.h"

void URPGDragVisualWidget::SetDragIcon(class UTexture2D* InTexture)
{
	if (DragIcon && InTexture)
	{
		DragIcon->SetBrushFromTexture(InTexture);
		// (可选) 拖拽时的图标稍微给一点透明度，手感更好
		DragIcon->SetRenderOpacity(0.8f);
	}
}
