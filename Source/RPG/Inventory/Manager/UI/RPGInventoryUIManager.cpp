// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGInventoryUIManager.h"

#include "RPG/Inventory/Manager/Asset/RPGInvetoryAssetManager.h"
#include "RPG/Player/RPGPlayerState.h"

void URPGInventoryUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void URPGInventoryUIManager::Deinitialize()
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
	}
	
	Super::Deinitialize();
}

void URPGInventoryUIManager::ToggleInventoryUI()
{
	if (InventoryWidget && InventoryWidget->IsInViewport())
	{
		CloseInventoryUI();
	}
	else
	{
		OpenInventoryUI();
	}
}

void URPGInventoryUIManager::OpenInventoryUI()
{
	APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld());
	if (!PlayerController) return;

	// 1. 懒加载：如果内存里还没有这个 Widget，先把它造出来并初始化
	if (!InventoryWidget)
	{
		// 安全加载 Class
		UClass* InventoryWidgetClass = URPGInvetoryAssetManager::Get().InventoryWidgetClass.LoadSynchronous();
		if (!InventoryWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("OpenInventoryUI Failed: InventoryWidgetClass is Null!"));
			return;
		}

		// 创建 Widget
		InventoryWidget = CreateWidget<URPGInventoryWidget>(PlayerController, InventoryWidgetClass);
          
		// 安全获取 Component 并初始化
		if (InventoryWidget)
		{
			// 严格的防御性指针层级检查
			if (APlayerState* PlayerState = PlayerController->GetPlayerState<ARPGPlayerState>())
			{
				if (URPGInventoryComponent* InventoryComponent = PlayerState->FindComponentByClass<URPGInventoryComponent>())
				{
					InventoryWidget->InitializeInventoryWidget(InventoryComponent);
				}
			}
		}
	}

	// 2. 显示逻辑：无论是不是刚创建的，既然叫 Open，就必须保证它显示在屏幕上
	if (InventoryWidget && !InventoryWidget->IsInViewport())
	{
		InventoryWidget->AddToViewport();
        
		// 如果你之前分离了输入模式控制，记得在这里调用它
		UpdateInputMode(true); 
	}
}

void URPGInventoryUIManager::CloseInventoryUI() const
{
	InventoryWidget->RemoveFromParent();
	UpdateInputMode(false);
}

void URPGInventoryUIManager::UpdateInputMode(const bool bIsUIOpen) const
{
	APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld());
	if (!PlayerController) return;
	
	if (bIsUIOpen)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}
