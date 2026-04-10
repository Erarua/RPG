// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UBasicAttributeSet::UBasicAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	Damage = 0.f;
	Shield = 0.f;
	MaxShield = 100.f;
}

void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxShield());
	}
}

void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (GetDamage() > 0.f)
		{
			FGameplayTagContainer DebuffTagContainer;
			Data.EffectSpec.GetAllGrantedTags(DebuffTagContainer);
			if (!DebuffTagContainer.HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Debuff"))))
			{
				// If the damage effect do not have a debuff tag, we consider it as a hit and try to activate hit reaction abilities
				FGameplayTagContainer HitReactionTagContainer;
				HitReactionTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.HitReaction")));
				GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(HitReactionTagContainer);
			}
			
			float DamageToApply = GetDamage();
			
			if (GetShield() > 0.f)
			{
				float ShieldAbsorb = FMath::Min(GetShield(), DamageToApply);
				SetShield(GetShield() - ShieldAbsorb);
				DamageToApply -= ShieldAbsorb;
			}
			
			if (DamageToApply > 0.f)
			{
				SetHealth(GetHealth() - DamageToApply);
			}
			
			SetDamage(0.f);
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());
		
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());
	}
}

void UBasicAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetHealthAttribute() && NewValue <= 0.f)
	{
		FGameplayTagContainer DeathTagContainer;
		DeathTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayAbility.Death")));
		GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(DeathTagContainer);
	}
	
	
}
