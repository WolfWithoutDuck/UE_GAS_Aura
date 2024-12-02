// 版权声明--


#include "AbilitySystem/Abilitys/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "aura/AuraLogChannels.h"
#include "Interation/CombatInterface.h"

/*
 *根据当前GA上填写的数值，生成GE应用目标造成伤害
 */
void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (auto Pair : DamageTypes)
	{
		const float ScaleDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaleDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
	                                                                          UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

/**
 * 根据当前的伤害Ability。可生成一个伤害的Debuff。
 * 用于动态生成的伤害GE，应用到目标身上
 * 传入Actor用于计算击飞方向 TODO:设计有问题，施加伤害时候如果有击退，但是没有几率，还是会击退
 * @param TargetActor
 * @param InRadialDamageOrigin
 * @param bOverrideKnockbackDirection
 * @param bOverrideDeathImpulse
 * @param DeathImpulseDirectionOverride
 * @param bOverridePitch
 * @param PitchOverride 
 * @return 
 */
FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor, FVector InRadialDamageOrigin,
                                                                                        bool bOverrideKnockbackDirection,
                                                                                        FVector KnockbackDirectionOverride,
                                                                                        bool bOverrideDeathImpulse,
                                                                                        FVector DeathImpulseDirectionOverride,
                                                                                        bool bOverridePitch,
                                                                                        float PitchOverride)
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);


	//默认击退
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 15.f;
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
	}

	//覆写击退
	if (bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		Params.KnockbackForce = KnockbackDirectionOverride * KnockbackForceMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			Params.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
		}
	}

	//覆写死亡击退
	if (bOverrideDeathImpulse)
	{
		DeathImpulseDirectionOverride.Normalize();
		Params.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}


	//这是自己修改的真实的伤害值,读表获取，怪物还没有实现
	Params.InflictDamage = GetCauseDamage();
	Params.AbilityLevel = GetAbilityLevel();

	// Params.DebuffChance = DebuffChance;
	// Params.DebuffDamage = DebuffDamage;
	// Params.DebuffDuration = DebuffDuration;
	// Params.DebuffFrequency = DebuffFrequency;


	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;


	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}

	return Params;
}

//返回当前总点燃几率
float UAuraDamageGameplayAbility::GetIgniteChance()
{
	const UAuraAttributeSet* Attributes = ICombatInterface::Execute_GetAttributeSet(GetAvatarActorFromActorInfo());
	const float IgniteChance = Attributes->GetIgniteChance();
	const float InLocalIgniteChance = GetLocalIgniteChanceInc();
	return IgniteChance + InLocalIgniteChance;
}

//当前本地技能点燃几率增加
float UAuraDamageGameplayAbility::GetLocalIgniteChanceInc()
{
	return LocalIgniteChance;
}

//返回当前总持续时间
float UAuraDamageGameplayAbility::GetIgniteDuration()
{
	const UAuraAttributeSet* Attributes = ICombatInterface::Execute_GetAttributeSet(GetAvatarActorFromActorInfo());
	const float BaseDuration = Attributes->GetIgniteDuration();
	const float DurationInc = Attributes->GetIgniteDurationIncrease() / 100;
	const float LocalDurationInc = GetLocalIgniteDurationInc() / 100;
	const float DurationMore = Attributes->GetIgniteDurationMore() / 100;
	const float LocalDurationMore = GetLocalIgniteDurationMore() / 100;
	const float IgniteDealDamageFaster = Attributes->GetIgniteDealDamageFaster() / 100;
	const float FinalDuration = BaseDuration * (1 + DurationInc + LocalDurationInc) * (1 + DurationMore + LocalDurationMore) / (1 + IgniteDealDamageFaster);
	UE_LOG(LogAura, Warning, TEXT("造成点燃伤害 -->点燃持续时间->基础持续时间=[%f]->持续Inc=[%f]->本地持续Inc=[%f]->持续More=[%f]->本地持续More=[%f]->持续加速=[%f]->总持续=[%f] "), BaseDuration, DurationInc, LocalDurationInc, DurationMore,
	       LocalDurationMore, IgniteDealDamageFaster, FinalDuration);
	return FinalDuration;
}

//返回当前点燃周期
float UAuraDamageGameplayAbility::GetIgnitePeriod()
{
	const UAuraAttributeSet* Attributes = ICombatInterface::Execute_GetAttributeSet(GetAvatarActorFromActorInfo());
	const float IgniteDealDamageFaster = Attributes->GetIgniteDealDamageFaster() / 100;
	const float IgnitePeriod = 1 / (1 + IgniteDealDamageFaster);
	UE_LOG(LogAura, Warning, TEXT("点燃周期 -->[%f]"), IgnitePeriod)
	return IgnitePeriod;
}

//全局点燃持续时间增加
float UAuraDamageGameplayAbility::GetIgniteDurationInc()
{
	const UAuraAttributeSet* Attributes = ICombatInterface::Execute_GetAttributeSet(GetAvatarActorFromActorInfo());
	return Attributes->GetIgniteDurationIncrease();
}

//本地点燃持续时间增加
float UAuraDamageGameplayAbility::GetLocalIgniteDurationInc()
{
	return LocalIgniteDurationIncrease;
}

//全局点燃持续时间总增
float UAuraDamageGameplayAbility::GetIgniteDurationMore()
{
	const UAuraAttributeSet* Attributes = ICombatInterface::Execute_GetAttributeSet(GetAvatarActorFromActorInfo());
	return Attributes->GetIgniteDurationMore();
}

//本地点燃持续时间总增
float UAuraDamageGameplayAbility::GetLocalIgniteDurationMore()
{
	return LocalIgniteDurationMore;
}

TMap<FGameplayTag, float> UAuraDamageGameplayAbility::GetIgniteDamage()
{
	return TMap<FGameplayTag, float>();
}

TMap<FGameplayTag, float> UAuraDamageGameplayAbility::GetCauseDamage()
{
	return TMap<FGameplayTag, float>();
}

TMap<FGameplayTag, float> UAuraDamageGameplayAbility::GetBaseAndAddedDamage()
{
	return TMap<FGameplayTag, float>();
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}
