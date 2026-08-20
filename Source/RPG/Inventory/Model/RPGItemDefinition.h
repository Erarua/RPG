// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGItemDefinition.generated.h"
class URPGItemFragment;
/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class RPG_API URPGItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	
	URPGItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<URPGItemFragment>> Fragments;
	
	
public:
	
	const URPGItemFragment* FindFragmentByClass(TSubclassOf<URPGItemFragment> FragmentClass) const;
	
};
