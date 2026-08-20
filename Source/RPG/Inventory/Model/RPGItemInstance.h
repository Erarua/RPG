// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagStack.h"
#include "../Data/RPGInventorySlotData.h"
#include "RPGItemInstance.generated.h"


class URPGItemFragment;
struct FGameplayTagStackArray;
/**
 * 
 */
UCLASS(BlueprintType)
class URPGItemInstance : public UObject
{
	GENERATED_BODY()
	
	
public:
	
	URPGItemInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual bool IsSupportedForNetworking() const override {return true;}
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddStack(FGameplayTag Tag, int32 StackCount);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveStack(FGameplayTag Tag, int32 StackCount);
	
	UFUNCTION(BlueprintCallable, Category=Inventory)
	int32 GetStackCount(FGameplayTag Tag) const;
	
	UFUNCTION(BlueprintCallable, Category=Inventory)
	bool ContainsTag(FGameplayTag Tag) const;
	
	void SetItemDefinition(TSubclassOf<class URPGItemDefinition> InItemDef);
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const URPGItemFragment* FindFragmentByClass(TSubclassOf<URPGItemFragment> FragmentClass) const;

	TSubclassOf<class URPGItemDefinition> GetItemDefinition() const
	{
		return ItemDef;
	}
	
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}
	
	
private:
	friend class URPGInventoryComponent;
	
	UPROPERTY(Replicated)
	TSubclassOf<class URPGItemDefinition> ItemDef;
	
	UPROPERTY(Replicated)
	FGameplayTagStackArray TagStackArray;
	
};
