// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffectTypes.h"
#include "Components/ActorComponent.h"
#include "RPG/Inventory/Data/RPGInventorySlotData.h"
#include "RPG/Inventory/Model/RPGItemInstance.h"
#include "RPGInventoryComponent.generated.h"


class URPGItemInstance;
struct FRPGInventorySlotData;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlotDataUpdated, int32, const FRPGInventorySlotData&);


USTRUCT(BlueprintType)
struct FRPGInstanceData
{
	GENERATED_BODY()
	
private:
	
	friend class URPGInventoryComponent;
	
	UPROPERTY()
	TObjectPtr<URPGItemInstance> Instance = nullptr;
	
	UPROPERTY()
	FRPGInventorySlotData SlotData;
	
	UPROPERTY(NotReplicated)
	FGameplayAbilitySpecHandle ActiveAbilitySpecHandle;
	
	UPROPERTY(NotReplicated)
	TArray<FGameplayAbilitySpecHandle> PassiveAbilitySpecHandle;
	
	UPROPERTY(NotReplicated)
	TArray<FActiveGameplayEffectHandle> PassiveEffectSpecHandle;
	
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPG_API URPGInventoryComponent : public UActorComponent
{

private:
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URPGInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SwapSlots(int32 SourceIndex, int32 TargetIndex);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_UseItem(int32 SlotIndex);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_RemoveItem(int32 SlotIndex);
	
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	virtual void ReadyForReplication() override;
	
	
	int32 GetCapacity() const
	{
		return Capacity;
	}

	void SetCapacity(int32 InCapacity)
	{
		this->Capacity = InCapacity;
	}

	int32 GetMaxCapacity() const
	{
		return MaxCapacity;
	}

	int32 GetMinCapacity() const
	{
		return MinCapacity;
	}

	int32 GetNumColumns() const
	{
		return NumColumns;
	}

	int32 GetNumRows() const
	{
		return NumRows;
	}
	
	FRPGInventorySlotData GetSlotData(int32 Index)
	{
		return Slots.IsValidIndex(Index) ? Slots[Index].SlotData : FRPGInventorySlotData();
	}
	
	void OnItemAdded(int32 SlotIndex, URPGItemInstance* Item);
	
	
public:
	
	FOnSlotDataUpdated OnSlotDataUpdated;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_AddRandomItem();
	

protected:
	
	UPROPERTY(ReplicatedUsing = OnRep_Slots)
	TArray<FRPGInstanceData> Slots;
	
	UFUNCTION()
	void OnRep_Slots(const TArray<FRPGInstanceData>& OldSlots);
	
	UAbilitySystemComponent* ResolveASC() const;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxCapacity = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MinCapacity = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumColumns = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumRows = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 Capacity = 0;
	
	
private:
	
	UFUNCTION(BLueprintCallable)
	bool SwapSlots(int32 SourceIndex, int32 TargetIndex);
	
	UFUNCTION()
	void OnSlotDataSwapped(int32 SourceIndex, int32 TargetIndex);
	
	UFUNCTION()
	void AddItem(FName ItemID);
	
	UFUNCTION()
	void OnItemRemoved(int32 SlotIndex);
	
	UFUNCTION()
	void OnItemUsed(int32 SlotIndex);
	
};
