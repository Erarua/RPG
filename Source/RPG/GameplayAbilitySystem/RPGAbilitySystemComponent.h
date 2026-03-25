// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
protected:
	
	TArray<FGameplayAbilitySpec> LastActivatableAbilities;

public:
	// Sets default values for this component's properties
	URPGAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRep_ActivateAbilities() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
