// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGInventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Asset/RPGInvetoryAssetManager.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "RPG/Inventory/Data/RPGItemData.h"
#include "RPG/Inventory/Model/RPGItemDefinition.h"
#include "RPG/Inventory/Model/RPGItemFragment_ConsumableItem.h"
#include "RPG/Inventory/Model/RPGItemFragment_EquippableItem.h"
#include "RPG/Inventory/Model/RPGItemFragment_StackableItem.h"


// Sets default values for this component's properties
URPGInventoryComponent::URPGInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPGInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	Slots.Empty();
	Slots.SetNum(MaxCapacity);

	for (int32 i = MinCapacity; i < MaxCapacity; ++i)
	{
		if (Capacity <= i && i < MaxCapacity)
		{
			Slots[i].SlotData.bIsLocked = true;
		}
	}
}


// Called every frame
void URPGInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URPGInventoryComponent::Server_UseItem_Implementation(int32 SlotIndex)
{
	if (!GetOwner()->HasAuthority()) return;
	if (!Slots.IsValidIndex(SlotIndex)) return;
	
	URPGItemInstance* Instance = Slots[SlotIndex].Instance;
	if (!IsValid(Instance)) return;
	if (!Instance) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Server_UseItem"));
	
	if (Slots[SlotIndex].ActiveAbilitySpecHandle.IsValid())
	{
		if (UAbilitySystemComponent* ASC = ResolveASC())
		{
			ASC->TryActivateAbility(Slots[SlotIndex].ActiveAbilitySpecHandle);
			OnItemUsed(SlotIndex);
		}
	}
}

void URPGInventoryComponent::Server_RemoveItem_Implementation(int32 SlotIndex)
{
	TRACE_BOOKMARK(TEXT("Server_RemoveItem Begins"));
	if (!GetOwner()->HasAuthority()) return;
	if (!Slots.IsValidIndex(SlotIndex)) return;
	
	OnItemRemoved(SlotIndex);
	
	UE_LOG(LogTemp, Warning, TEXT("Server_RemoveItem"));
	TRACE_BOOKMARK(TEXT("Server_RemoveItem Ends"));
}

bool URPGInventoryComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                                 FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (int32 i = MinCapacity; i < MaxCapacity; ++i)
	{
		URPGItemInstance* Instance = Slots[i].Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void URPGInventoryComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	if (IsUsingRegisteredSubObjectList())
	{
		for (int32 i = MinCapacity; i < MaxCapacity; ++i)
		{
			URPGItemInstance* Instance = Slots[i].Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

void URPGInventoryComponent::Server_SwapSlots_Implementation(int32 SourceIndex, int32 TargetIndex)
{
	FString BookMarkMsg = FString::Printf(TEXT("Server_SwapSlots Begins in %s"), *GetOwner()->GetName());
	TRACE_BOOKMARK(TEXT("Server_SwapSlots Begins"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), *BookMarkMsg);
	if (!GetOwner()->HasAuthority()) return;
	SwapSlots(SourceIndex, TargetIndex);
}

bool URPGInventoryComponent::SwapSlots(int32 SourceIndex, int32 TargetIndex)
{
	if (GetOwner() && !GetOwner()->HasAuthority())
	{
		return false;
	}

	if (!Slots.IsValidIndex(SourceIndex) || !Slots.IsValidIndex(TargetIndex)) return false;
	if (SourceIndex == TargetIndex) return true;
	if (Slots[SourceIndex].SlotData.bIsLocked || Slots[TargetIndex].SlotData.bIsLocked) return false;
	TRACE_BOOKMARK(TEXT("SwapSlots Begins"));

	// UE_LOG(LogTemp, Warning, TEXT("Swapping slots: SourceIndex=%d, TargetIndex=%d"), SourceIndex, TargetIndex);
	// UE_LOG(LogTemp, Warning, TEXT("SourceData: ItemID=%s, Amount=%i Locked=%s"), *Slots[SourceIndex].SlotData.ItemID.ToString(), Slots[SourceIndex].SlotData.Amount, Slots[TargetIndex].SlotData.bIsLocked ? TEXT("true") : TEXT("false"));
	// UE_LOG(LogTemp, Warning, TEXT("TargetData: ItemID=%s, Amount=%i Locked=%s"), *Slots[TargetIndex].SlotData.ItemID.ToString(), Slots[TargetIndex].SlotData.Amount, Slots[TargetIndex].SlotData.bIsLocked ? TEXT("true") : TEXT("false"));
	
	
	Slots.Swap(SourceIndex, TargetIndex);
	if (AActor* Owner = GetOwner())
	{
		Owner->ForceNetUpdate();
	}
	

	// UE_LOG(LogTemp, Warning, TEXT("Swapped slots: SourceIndex=%d, TargetIndex=%d"), SourceIndex, TargetIndex);
	// UE_LOG(LogTemp, Warning, TEXT("SourceData: ItemID=%s, Amount=%i Locked=%s"), *Slots[SourceIndex].SlotData.ItemID.ToString(), Slots[SourceIndex].SlotData.Amount, Slots[TargetIndex].SlotData.bIsLocked ? TEXT("true") : TEXT("false"));
	// UE_LOG(LogTemp, Warning, TEXT("TargetData: ItemID=%s, Amount=%i Locked=%s"), *Slots[TargetIndex].SlotData.ItemID.ToString(), Slots[TargetIndex].SlotData.Amount, Slots[TargetIndex].SlotData.bIsLocked ? TEXT("true") : TEXT("false"));


	if (GetNetMode() != NM_DedicatedServer)
	{
		OnSlotDataUpdated.Broadcast(SourceIndex, Slots[SourceIndex].SlotData);
		OnSlotDataUpdated.Broadcast(TargetIndex, Slots[TargetIndex].SlotData);
		TRACE_BOOKMARK(TEXT("OnSlotDataSwapped Broadcast"));
		// UE_LOG(LogTemp, Warning, TEXT("Broadcasted slot data updated for SourceIndex=%d and TargetIndex=%d"), SourceIndex, TargetIndex);
	}
	
	TRACE_BOOKMARK(TEXT("Server_SwapSlots Ends"));
	FString BookMarkMsg = FString::Printf(TEXT("Server_SwapSlots Ends in %s"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *BookMarkMsg);

	return true;
}

void URPGInventoryComponent::OnItemAdded(int32 SlotIndex, URPGItemInstance* Item)
{
	// UE_LOG(LogTemp, Warning, TEXT("OnItemAdded Begin"));

	const URPGItemDefinition* ItemDefinition = GetDefault<URPGItemDefinition>(Item->ItemDef);
	for (const TObjectPtr<URPGItemFragment> Fragment : ItemDefinition->Fragments)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Processing Fragment: %s"), *Fragment->GetName());
		Fragment->OnInstanceCreated(Item);
	}

	const URPGItemFragment_EquippableItem* EquippableFragment = Item->FindFragmentByClass<
		URPGItemFragment_EquippableItem>();
	if (!EquippableFragment)
	{
		return;
	}

	// UE_LOG(LogTemp, Warning, TEXT("Accessing ASC"));

	if (UAbilitySystemComponent* ASC = ResolveASC())
	{
		// UE_LOG(LogTemp, Warning, TEXT("OnItemAdded: Giving abilities to ASC"));
		if (EquippableFragment->ActiveAbilityClass)
		{
			Slots[SlotIndex].ActiveAbilitySpecHandle = ASC->GiveAbility(
				FGameplayAbilitySpec(EquippableFragment->ActiveAbilityClass, 1, INDEX_NONE, Item));
		}

		for (const auto& EffectSetting : EquippableFragment->PassiveEffectSetting)
		{
			TSubclassOf<UGameplayEffect> EffectClass = EffectSetting->GameplayEffectClass;
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->
				MakeOutgoingSpec(EffectClass, 1, ASC->MakeEffectContext());
			check(EffectSpecHandle.IsValid());
			FGameplayEffectSpec* EffectSpec = EffectSpecHandle.Data.Get();
			for (const auto& Attribute : EffectSetting->SetByCallerAttributes)
			{
				EffectSpec->SetSetByCallerMagnitude(Attribute.Key, Attribute.Value);
				// UE_LOG(LogTemp, Warning, TEXT("Setting SetByCaller attribute: %s with magnitude: %f"), *Attribute.Key.ToString(), Attribute.Value);
			}
			Slots[SlotIndex].PassiveEffectSpecHandle.Add(ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec));
		}

		for (const TSubclassOf<UGameplayAbility>& AbilityClass : EquippableFragment->PassiveAbilityClasses)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability Given: %s"), AbilityClass ? *AbilityClass->GetName() : TEXT("null"));
			FGameplayAbilitySpecHandle SpecHandle = ASC->GiveAbility(
				FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, Item));
			Slots[SlotIndex].PassiveAbilitySpecHandle.Add(SpecHandle);
			UE_LOG(LogTemp, Warning, TEXT("SpecHandle: %s"), *SpecHandle.ToString());
		}
	}
}

void URPGInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPGInventoryComponent, Slots);
}

void URPGInventoryComponent::Server_AddRandomItem_Implementation()
{
	TRACE_BOOKMARK(TEXT("Server_AddRandomItem Begins"));
	FString BookMarkMsg = FString::Printf(TEXT("Server_AddRandomItem Begins in %s"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *BookMarkMsg);
	// UE_LOG(LogTemp, Warning, TEXT("Server_AddRandomItem_Implementation"));
	const UDataTable* ItemTable = URPGInvetoryAssetManager::Get().GetItemTable();
	if (!ItemTable)
	{
		// UE_LOG(LogTemp, Error, TEXT("No Item Table found"));
		return;
	}
	TArray<FName> ItemNames = ItemTable->GetRowNames();
	if (ItemNames.Num() == 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("No item names found"));
		return;
	}

	int32 RandomIdx = FMath::RandRange(0, ItemNames.Num() - 1);
	FName ItemID = ItemNames[RandomIdx];

	AddItem(ItemID);
	
	TRACE_BOOKMARK(TEXT("Server_AddRandomItem Ends"));
	BookMarkMsg = FString::Printf(TEXT("Server_AddRandomItem Ends in %s"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *BookMarkMsg);
}

void URPGInventoryComponent::OnRep_Slots(const TArray<FRPGInstanceData>& OldSlots)
{
	// UE_LOG(LogTemp, Warning, TEXT("[%s] OnRep_Slots"), *FDateTime::UtcNow().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Slots.Num()=%d, OldSlots.Num()=%d"), Slots.Num(), OldSlots.Num());
	
	TRACE_BOOKMARK(TEXT("OnRep_Slots Begins"));
	FString BookMarkMsg = FString::Printf(TEXT("OnRep_Slots Begins in %s"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *BookMarkMsg);

	if (Slots.IsEmpty() || OldSlots.IsEmpty())
	{
		return;
	}

	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (!OldSlots.IsValidIndex(i))
		{
			return;
		}

		// UE_LOG(LogTemp, Warning, TEXT("Checking slot %d"), i);
		// if (Slots[i].Instance == nullptr)
		// {
		// 	// UE_LOG(LogTemp, Warning, TEXT("Slot %d is null"), i);
		//
		// 	continue;
		// }
		// if (OldSlots[i].Instance == nullptr)
		// {
		// 	// UE_LOG(LogTemp, Warning, TEXT("OldSlot %d is null"), i);
		// 	OnSlotDataUpdated.Broadcast(i, Slots[i].SlotData);
		// 	continue;
		// }
		const FRPGInventorySlotData OldData = OldSlots[i].SlotData;
		const FRPGInventorySlotData NewData = Slots[i].SlotData;
		if (OldData.ItemID != NewData.ItemID || NewData.Amount != OldData.Amount || NewData.bIsLocked != OldData.
			bIsLocked)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Inventory Slot ID: %i Update. OldID=%s, NewID=%s"), i, *OldDef->ItemID.ToString(), *NewDef->ItemID.ToString());
			// UE_LOG(LogTemp, Warning, TEXT("Inventory Slot Amount: %i Update. Old=%i, New=%i"), i, OldSlots[i].SlotData.Amount, Slots[i].SlotData.Amount);
			// UE_LOG(LogTemp, Warning, TEXT("Inventory Slot Locked: %i Updated. Old=%i, New=%i"), i, OldSlots[i].SlotData.bIsLocked, Slots[i].SlotData.bIsLocked);
			UE_LOG(LogTemp, Warning, TEXT("Instance: %s"),
			       Slots[i].Instance ? *Slots[i].Instance->GetName() : TEXT("null"));

			OnSlotDataUpdated.Broadcast(i, Slots[i].SlotData);
			for (const FGameplayAbilitySpecHandle SpecHandle : Slots[i].PassiveAbilitySpecHandle)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *SpecHandle.ToString());
			}
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("[%s] OnRep_Slots Finished"), *FDateTime::UtcNow().ToString());
	
	TRACE_BOOKMARK(TEXT("OnRep_Slots Ends"));
	BookMarkMsg = FString::Printf(TEXT("OnRep_Slots Ends in %s"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *BookMarkMsg);
}

UAbilitySystemComponent* URPGInventoryComponent::ResolveASC() const
{
	const APlayerState* PlayerState = Cast<APlayerState>(GetOwner());
	if (!PlayerState) return nullptr;

	APawn* Pawn = PlayerState->GetPawn();
	if (!Pawn) return nullptr;

	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Pawn))
	{
		return AbilitySystemInterface->GetAbilitySystemComponent();
	}
	return Pawn->FindComponentByClass<UAbilitySystemComponent>();
}

void URPGInventoryComponent::OnSlotDataSwapped(int32 SourceIndex, int32 TargetIndex)
{
	SwapSlots(SourceIndex, TargetIndex);
}

void URPGInventoryComponent::AddItem(FName ItemID)
{
	FRPGItemData* ItemData = URPGInvetoryAssetManager::GetItem(ItemID);
	const URPGItemDefinition* ItemDefinition = GetDefault<URPGItemDefinition>(ItemData->ItemDef);

	if (!ItemData) return;
	int32 Idx = 0;
	if (const URPGItemFragment_StackableItem* StackableItem = Cast<URPGItemFragment_StackableItem>(
		ItemDefinition->FindFragmentByClass(URPGItemFragment_StackableItem::StaticClass())))
	{
		while (Idx < Slots.Num())
		{
			if (Slots[Idx].SlotData.bIsLocked)
			{
				break;
			}
			if (Slots[Idx].SlotData.ItemID == ItemID)
			{
				if (Slots[Idx].SlotData.Amount < StackableItem->MaxStack)
				{
					Slots[Idx].SlotData.Amount++;
					if (AActor* Owner = GetOwner())
					{
						Owner->ForceNetUpdate();
					}
					OnSlotDataUpdated.Broadcast(Idx, Slots[Idx].SlotData);
					TRACE_BOOKMARK(TEXT("OnSlotDataAdded Broadcast"));
				}
				return;
			}
			Idx++;
		}
	}
	Idx = 0;
	while (Idx < Slots.Num())
	{
		if (Slots[Idx].SlotData.bIsLocked)
		{
			break;
		}
		if (Slots[Idx].SlotData.IsEmpty())
		{
			URPGItemInstance* ItemInstance = NewObject<URPGItemInstance>(this);
			ItemInstance->ItemDef = ItemData->ItemDef;
			Slots[Idx].Instance = ItemInstance;
			// for (const URPGItemFragment* Fragment : ItemDefinition->Fragments)
			// {
			// 	Fragment->OnInstanceCreated(ItemInstance);
			// }

			Slots[Idx].SlotData.Amount = 1;
			Slots[Idx].SlotData.ItemID = ItemID;
			// UE_LOG(LogTemp, Warning, TEXT("Item ID=%s, Item Instance = %s"), *ItemID.ToString(), *ItemInstance->GetName());
			OnItemAdded(Idx, ItemInstance);
			if (AActor* Owner = GetOwner())
			{
				Owner->ForceNetUpdate();
			}
			OnSlotDataUpdated.Broadcast(Idx, Slots[Idx].SlotData);
			TRACE_BOOKMARK(TEXT("OnSlotDataAdded Broadcast"));
			break;
		}
		Idx++;
	}
}

void URPGInventoryComponent::OnItemRemoved(int32 SlotIndex)
{
	if (!Slots.IsValidIndex(SlotIndex)) return;
	
	Slots[SlotIndex].Instance = nullptr;
	Slots[SlotIndex].SlotData.Amount = 0;
	Slots[SlotIndex].SlotData.ItemID = NAME_None;
	
	if (UAbilitySystemComponent* ASC = ResolveASC())
	{
		if (Slots[SlotIndex].ActiveAbilitySpecHandle.IsValid())
		{
			ASC->ClearAbility(Slots[SlotIndex].ActiveAbilitySpecHandle);
			Slots[SlotIndex].ActiveAbilitySpecHandle = FGameplayAbilitySpecHandle();
		}

		for (const FActiveGameplayEffectHandle& EffectHandle : Slots[SlotIndex].PassiveEffectSpecHandle)
		{
			if (EffectHandle.IsValid())
			{
				ASC->RemoveActiveGameplayEffect(EffectHandle);
			}
		}
		Slots[SlotIndex].PassiveEffectSpecHandle.Empty();

		for (const FGameplayAbilitySpecHandle& AbilityHandle : Slots[SlotIndex].PassiveAbilitySpecHandle)
		{
			if (AbilityHandle.IsValid())
			{
				ASC->ClearAbility(AbilityHandle);
			}
		}
		Slots[SlotIndex].PassiveAbilitySpecHandle.Empty();
	}
	if (AActor* Owner = GetOwner())
	{
		Owner->ForceNetUpdate();
	}
	OnSlotDataUpdated.Broadcast(SlotIndex, Slots[SlotIndex].SlotData);
}

void URPGInventoryComponent::OnItemUsed(int32 SlotIndex)
{
	if (!Slots.IsValidIndex(SlotIndex)) return;
	
	if (const URPGItemFragment_ConsumableItem* ConsumableFragment = Slots[SlotIndex].Instance->FindFragmentByClass<URPGItemFragment_ConsumableItem>())
	{
		Slots[SlotIndex].SlotData.Amount -= ConsumableFragment->ConsumeStack;
		if (Slots[SlotIndex].SlotData.Amount <= 0)
		{
			OnItemRemoved(SlotIndex);
		}
		else
		{
			if (AActor* Owner = GetOwner())
			{
				Owner->ForceNetUpdate();
			}
			OnSlotDataUpdated.Broadcast(SlotIndex, Slots[SlotIndex].SlotData);
		}
	}
}
