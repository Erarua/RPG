// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "StructUtils/UserDefinedStruct.h"
#include "GameplayTagStack.generated.h"

struct FGameplayTagStackArray;
struct FGameplayTag;
/**
 * 
 */
USTRUCT(BlueprintType)
struct RPG_API FGameplayTagStack: public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	FGameplayTagStack()
	{}
	
	FGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag), StackCount(InStackCount)
	{}
	
private:
	
	friend FGameplayTagStackArray;
	
	UPROPERTY()
	FGameplayTag Tag;
	
	UPROPERTY()
	int32 StackCount = 0;
};


USTRUCT(BlueprintType)
struct RPG_API FGameplayTagStackArray: public FFastArraySerializer
{
	GENERATED_BODY()
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackArray>(Stacks, DeltaParms, *this);
	}
	
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	FGameplayTagStackArray()
	{}
	
	void AddStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStack(FGameplayTag Tag, int32 StackCount);
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}
	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}
	
private:
	
	UPROPERTY()
	TArray<FGameplayTagStack> Stacks;
	
	TMap<FGameplayTag, int32> TagToCountMap;
};

template<>
struct TStructOpsTypeTraits<FGameplayTagStackArray> : public TStructOpsTypeTraitsBase2<FGameplayTagStackArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};


