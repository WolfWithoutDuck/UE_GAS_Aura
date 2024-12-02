#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;


/**
 *DamageGE参数，用于动态生成伤害GE 然后Apply
 */
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams()
	{
	}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	//当前造成的直击伤害
	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayTag, float> InflictDamage;

	//当前造成的点燃伤害
	TMap<FGameplayTag, float> IgniteDamage;

	//点燃几率
	float IgniteChance = 0.f;

	//点燃持续时间
	float IgniteDuration = 4.f;

	//点燃周期
	float IgnitePeriod = 1.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	//范围伤害
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};


/**
 *自定义的GE上下文
 */
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	//设置获取当前GE的伤害信息
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	//设置获取当前GE是不是Debuff的上下文信息
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }


	bool GetIsSuccessfulIgnite() const { return bIsSuccessfulIgnite; }
	float GetIgniteDamage() const { return IgniteDamage; }
	float GetIgniteDuration() const { return IgniteDuration; }
	float GetIgnitePeriod() const { return IgnitePeriod; }

	void SetCriticalHit(bool IsCriticalHit) { bIsCriticalHit = IsCriticalHit; }
	void SetIsBlockedHit(bool IsBlockedHit) { bIsBlockedHit = IsBlockedHit; }
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetKnockbackForce(const FVector& InForce) { KnockbackForce = InForce; }
	void SetIsRadialDamage(const bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(const float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(const float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InDamageOrigin) { RadialDamageOrigin = InDamageOrigin; }

	void SetIsSuccessfulIgnite(const bool bInIsSuccessfulIgnite) { bIsSuccessfulIgnite = bInIsSuccessfulIgnite; }
	void SetIgniteDamage(const float InIgniteDamage) { IgniteDamage = InIgniteDamage; }
	void SetIgniteDuration(const float InIgniteDuration) { IgniteDuration = InIgniteDuration; }
	void SetIgnitePeriod(const float InIgnitePeriod) { IgnitePeriod = InIgnitePeriod; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	void SetIgniteStackInfo(float IgniteDamage, float IgniteEndTime);
	TMap<float, float>& GetIgniteCauseDamage();
	void CleanUpExpiredEffects(float CurrentTime);
	float GetTotalIgniteDamage(float CurrentTime, int32 IgniteStack);
	void InitIgniteStackInfo(const TMap<float, float>& IgniteDamageStack);

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;

	//是否造成点燃
	UPROPERTY()
	bool bIsSuccessfulIgnite = false;

	UPROPERTY()
	float IgniteDamage = 0.f;

	UPROPERTY()
	float IgniteDuration = 0.f;

	UPROPERTY()
	float IgnitePeriod = 1.f;

	UPROPERTY()
	TMap<float, float> IgniteDamageToEndTime;
};


template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
