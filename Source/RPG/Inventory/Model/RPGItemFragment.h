// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPGItemFragment.generated.h"

class URPGItemInstance;
/**
 * 
 */
UCLASS(MinimalAPI, DefaultToInstanced, EditInlineNew, Abstract) 
class URPGItemFragment : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual void OnInstanceCreated(URPGItemInstance* Instance) const {}
	
	virtual void OnInstanceDestroyed(URPGItemInstance* Instance) const {}
};
