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

	//感电持续时间
	float ShockDuration = 2.f;


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

	//点燃
	void SetIsSuccessfulIgnite(const bool bInIsSuccessfulIgnite) { bIsSuccessfulIgnite = bInIsSuccessfulIgnite; }
	void SetIgniteDamage(const float InIgniteDamage) { IgniteDamage = InIgniteDamage; }
	void SetIgniteDuration(const float InIgniteDuration) { IgniteDuration = InIgniteDuration; }
	void SetIgnitePeriod(const float InIgnitePeriod) { IgnitePeriod = InIgnitePeriod; }

	//感电
	void SetIsSuccessfulShock(const bool bInIsSuccessfulShock) { bIsSuccessfulShock = bInIsSuccessfulShock; }
	void SetShockEffect(const float InShockEffect) { ShockEffect = InShockEffect; }
	void SetShockDuration(const float InShockDuration) { ShockDuration = InShockDuration; }

	bool GetIsSuccessfulShock() const { return bIsSuccessfulShock; }
	float GetShockEffect() const { return ShockEffect; }
	float GetShockDuration() const { return ShockDuration; }


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

	//点燃伤害Buff堆栈
	UPROPERTY()
	TMap<float, float> IgniteDamageToEndTime;

	//感电相关
	//是否感电
	UPROPERTY()
	bool bIsSuccessfulShock = false;

	//感电层数
	UPROPERTY()
	float ShockEffect = 0.f;

	//感电持续时间
	UPROPERTY()
	float ShockDuration = 0.f;

	UPROPERTY()
	TMap<float, float> ShockEffectToEndTime;
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


/**
 *自定义的DebuffGE上下文
 */
USTRUCT(BlueprintType)
struct FDebuffGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FDebuffGameplayEffectContext* Duplicate() const
	{
		FDebuffGameplayEffectContext* NewContext = new FDebuffGameplayEffectContext();
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


	//初始覆盖所有Debuff数据
	void InitDebuffEffectStack(const TMap<float, float>& DebuffEffectStack);
	//根据传入的当前时间戳，清除所有过期的Debuff数据
	void CleanUpExpiredEffects(float CurrentTime);
	//传入DebuffEffect 和当前过期时间
	void SetDebuffEffectStackInfo(float InDebuffEffect, float InEffectEndTime);
	//获取当前的DebuffStack信息
	TMap<float, float>& GetDebuffEffectStackInfo();

	//通用设置Debuff信息
	void SetIsSuccessfulDebuff(const bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffEffect(const float InDebuffEffect) { DebuffEffect = InDebuffEffect; }
	void SetDebuffDuration(const float InDebuffDuration) { DebuffDuration = InDebuffDuration; }

	//通用获取Debuff信息
	bool GetIsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffEffect() const { return DebuffEffect; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetMaxDebuffEffect(float CurrentTime, int32 EffectStackCount);

protected:
	/*
	 *	Debuff堆栈，对应的数据以及结束时间，对应的数据可以是任何东西，比如
	 *	点燃--[点燃伤害，本次点燃伤害结束时间],只获取伤害最高的进行生效，当有多个点燃存在，获取最高的几个点燃生效
	 *	冰缓--[冰缓层数，本次冰缓层数结束时间]，只获取最高层数的冰缓生效
	 *	感电--[感电层数，本次感电层数结束时间]，只获取最高层数的感电生效
	 *	TODO 中毒
	 *	TODO 流血
	 *	
	 *	所有Debuff使用通用的规则，多个层数同一时间可以同时存在，但是应用规则可能略有不同，不同Buff再根据数据，自行进行处理
	 */
	UPROPERTY()
	TMap<float, float> DebuffEffectStackToEndTime;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffEffect = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffPeriod = 1.f;
};

template <>
struct TStructOpsTypeTraits<FDebuffGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FDebuffGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
