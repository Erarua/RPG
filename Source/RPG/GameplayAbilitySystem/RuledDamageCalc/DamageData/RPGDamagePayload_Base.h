#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGDamagePayload_Base.generated.h"

USTRUCT(BlueprintType)
struct FRPGDamagePayload_Base
{
	GENERATED_BODY()
	
	// Core Damage Data
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseHealthDamage = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseShieldDamage = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FinalHealthDamage = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FinalShieldDamage = 0.f;
	
	// Context Data
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> ContextData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> TargetAttributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, float> SourceAttributes;
	
};
