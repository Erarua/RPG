// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGItemDefinition.h"

#include "RPGItemFragment.h"

URPGItemDefinition::URPGItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

const URPGItemFragment* URPGItemDefinition::FindFragmentByClass(TSubclassOf<URPGItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (URPGItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	
	return nullptr;
}
