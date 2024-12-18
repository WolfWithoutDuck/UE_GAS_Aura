// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FTaggedMontage;
/**
 * 造成伤害的能力基类
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	static TMap<FGameplayTag, float> DefaultDamageMap;

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr, FVector InRadialDamageOrigin = FVector::ZeroVector,
	                                                            bool bOverrideKnockbackDirection = false,
	                                                            FVector KnockbackDirectionOverride = FVector::ZeroVector,
	                                                            bool bOverrideDeathImpulse = false,
	                                                            FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
	                                                            bool bOverridePitch = false,
	                                                            float PitchOverride = 0.f);

protected:
	/*
	 *需要使用这个GE来进行伤害的传递，这个GE定义了自定义的伤害计算类，在这个类里面进行伤害的计算执行
	 *比如抗性的伤害减免，暴击的增加伤害等
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/**
	 *当前GA定义了一个GE造成的伤害，比较简单，如果是复杂的多段伤害可能需要换一种方式
	 */
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;


	/**
	 * 返回当前造成的伤害
	 * @return 
	 */
	virtual TMap<FGameplayTag, float> GetCauseDamage();

	//Debuff信息
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDuration = 5.f;

	//死亡击飞
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeathImpulseMagnitude = 1000.f;

	//击退
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackForceMagnitude = 1000.f;

	//击退几率
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackChance = 0.f;

	//范围伤害
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float RadialDamageOuterRadius = 0.f;

	UFUNCTION
	(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages);
};
