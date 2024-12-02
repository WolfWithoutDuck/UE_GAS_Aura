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
	 * 获取当前技能的点燃几率，默认获取总的点燃几率属性（全局属性点燃几率+本地点燃几率）
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	virtual float GetIgniteChance();

	//当前技能本地增加的点燃几率
	UFUNCTION(BlueprintCallable)
	virtual float GetLocalIgniteChanceInc();
	
	/**
	 * 获取当前技能的点燃持续时间，获取总的持续时间，基础持续时间*（1+持续时间加成）*（1+持续时间总增加成）/(1+生效速度加成)
	 * 可被技能单独修正
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	virtual float GetIgniteDuration();

	//点燃持续时间总提高
	UFUNCTION(BlueprintCallable)
	virtual float GetIgniteDurationInc();

	//本地点燃持续时间总提高
	UFUNCTION(BlueprintCallable)
	virtual float GetLocalIgniteDurationInc();

	//点燃持续时间总增加
	UFUNCTION(BlueprintCallable)
	virtual float GetIgniteDurationMore();

	//本地点燃持续时间总增加
	UFUNCTION(BlueprintCallable)
	virtual float  GetLocalIgniteDurationMore();
	
	//点燃触发周期
	UFUNCTION(BlueprintCallable)
	virtual float GetIgnitePeriod();
	/**
	 * 获取当前技能的点燃伤害，默认为0
	 * 可被技能单独修正
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	virtual TMap<FGameplayTag, float> GetIgniteDamage();


	/**
	 * 返回当前造成的伤害
	 * @return 
	 */
	virtual TMap<FGameplayTag, float> GetCauseDamage();

	/**
	 * 返回当前技能的基础点伤和附加点伤，如果有多段伤害，则派生技能自行处理
	 * @return 
	 */
	virtual TMap<FGameplayTag, float> GetBaseAndAddedDamage();


	//本地点燃几率加成
	UPROPERTY(EditDefaultsOnly, Category="点燃")
	float LocalIgniteChance = 0.f;

	//本地点燃持续时间提高
	UPROPERTY(EditDefaultsOnly, Category="点燃")
	float LocalIgniteDurationIncrease = 0.f;

	//本地点燃持续时间总增
	UPROPERTY(EditDefaultsOnly, Category="点燃")
	float LocalIgniteDurationMore = 0.f;
	
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
