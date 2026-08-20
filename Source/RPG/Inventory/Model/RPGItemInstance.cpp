// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGItemInstance.h"

#include "RPGItemDefinition.h"
#include "RPGItemFragment.h"
#include "Net/UnrealNetwork.h"

URPGItemInstance::URPGItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URPGItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, TagStackArray);
}

void URPGItemInstance::AddStack(FGameplayTag Tag, int32 StackCount)
{
	TagStackArray.AddStack(Tag, StackCount);
}

void URPGItemInstance::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	TagStackArray.RemoveStack(Tag, StackCount);
}

int32 URPGItemInstance::GetStackCount(FGameplayTag Tag) const
{
	return TagStackArray.GetStackCount(Tag);
}

bool URPGItemInstance::ContainsTag(FGameplayTag Tag) const
{
	return TagStackArray.ContainsTag(Tag);
}

void URPGItemInstance::SetItemDefinition(TSubclassOf<URPGItemDefinition> InItemDef)
{
	ItemDef = InItemDef;
}

const URPGItemFragment* URPGItemInstance::FindFragmentByClass(TSubclassOf<URPGItemFragment> FragmentClass) const
{
	if (ItemDef && FragmentClass)
	{
		return GetDefault<URPGItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
