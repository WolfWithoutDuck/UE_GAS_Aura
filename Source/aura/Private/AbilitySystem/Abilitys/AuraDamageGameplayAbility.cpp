// 版权声明--


#include "AbilitySystem/Abilitys/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
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
 * @return 
 */
FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) 
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	//为了和教程一样先用临时值
	Params.BaseDamage = 0.f;
	Params.DamageType = FAuraGameplayTags::Get().Damage_Physical;
	
	//这是自己修改的真实的伤害值,读表获取，怪物还没有实现
	Params.BaseDamage2 = GetCauseDamage();
	Params.AbilityLevel = GetAbilityLevel();

	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;

	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 15.f;
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
	}

	return Params;
}

TMap<FGameplayTag, float> UAuraDamageGameplayAbility::GetCauseDamage()
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
