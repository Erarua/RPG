// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG/Inventory/WIdget/RPGInventoryWidget.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "RPGInventoryUIManager.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGInventoryUIManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	
public:
	
	UFUNCTION()
	void ToggleInventoryUI();
	

private:
	
	void OpenInventoryUI();
	
	void CloseInventoryUI() const;
	
	void UpdateInputMode(const bool bIsUIOpen) const;	


protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<URPGInventoryWidget> InventoryWidget;

	
};
