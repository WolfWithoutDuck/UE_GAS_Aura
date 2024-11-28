// 版权声明--

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/Character.h"
#include "AuraAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
	{
	}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;


/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();


	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// TMap<FGameplayTag, FGameplayAttribute(*)()> TagsToAttributes;
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/*
	 *	1.属性定义
	 */

	/*主属性  */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Strength, Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Intelligence, Category="Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Resilience, Category="Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Vigor, Category="Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);
	/*主属性  */

	/*次要属性  */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Armor, Category="Secondart Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ArmorPenetration, Category="Secondart Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_BlockChance, Category="Secondart Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitChance, Category="Secondart Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitDamage, Category="Secondart Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_CriticalHitResistance, Category="Secondart Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_HealthRegeneration, Category="Secondart Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ManaRegeneration, Category="Secondart Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxMana, Category="Secondart Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_MaxHealth, Category="Secondart Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	//伤害抗性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_FireResistance, Category="Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_LightingResistance, Category="Resistance Attributes")
	FGameplayAttributeData LightingResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightingResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ArcaneResistance, Category="Resistance Attributes")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_PhysicalResistance, Category="Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);


	//重要属性
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	//元属性
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP);

	/*次要属性 ~end*/

	/*点燃属性 ~~Start */
	//点燃几率
	UPROPERTY(BlueprintReadOnly, Category="Ignite Attributes")
	FGameplayAttributeData IgniteChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IgniteChance);

	//点燃持续时间
	UPROPERTY(BlueprintReadOnly, Category="Ignite Attributes")
	FGameplayAttributeData IgniteDuration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IgniteDuration);

	//点燃伤害生效速度
	UPROPERTY(BlueprintReadOnly, Category="Ignite Attributes")
	FGameplayAttributeData IgniteDealDamageFaster;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IgniteDealDamageFaster);

	/*点燃属性 ~~End	*/

	/*INC伤害属性  第一乘区 ~~Start */

	//伤害提高
	UPROPERTY(BlueprintReadOnly, Category="IncDamage Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Damage);

	//持续伤害提高
	UPROPERTY(BlueprintReadOnly, Category="IncDamage Attributes")
	FGameplayAttributeData DamageOverTime;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, DamageOverTime);
	/*INC伤害属性  第一乘区 ~~End */


	/*More伤害属性  第二乘区 ~~Start */
	//持续伤害提高
	UPROPERTY(BlueprintReadOnly, Category="MoreDamage Attributes")
	FGameplayAttributeData MoreBurningDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MoreBurningDamage);

	/*More伤害属性  第二乘区 ~~End */


	/*
	 * 2.设置哪些属性需要被同步
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/*
	 *  3.定义属性的复制函数
	 */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;

	UFUNCTION()
	void OnRep_LightingResistance(const FGameplayAttributeData& OldLightingResistance) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

	UFUNCTION()
	void OnRep_IgniteChance(const FGameplayAttributeData& OldIgniteChance) const;

	UFUNCTION()
	void OnRep_IgniteDuration(const FGameplayAttributeData& OldIgniteDuration) const;

	UFUNCTION()
	void OnRep_IgniteDealDamageFaster(const FGameplayAttributeData& OldIgniteDealDamageFaster) const;

	
	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldDamage) const;

	UFUNCTION()
	void OnRep_DamageOverTime(const FGameplayAttributeData& OldDamageOverTime) const;

	UFUNCTION()
	void OnRep_MoreBurningDamage(const FGameplayAttributeData& OldMoreBurningDamage) const;

private:
	void HandleInComingDamage(const FEffectProperties& Props);
	void HandleInComingXP(const FEffectProperties& Props);
	void Debuff(const FEffectProperties& Props);
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	static void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit);
	void SendXPEvent(const FEffectProperties& Props);
	bool bTopOffHealth;
	bool bTopOffMana;
};
