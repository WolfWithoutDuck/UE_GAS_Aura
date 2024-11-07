// 版权声明--


#include "AbilitySystem/Abilitys/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Actor/AuraProjectile.h"
#include "Interation/CombatInterface.h"


class UAbilityInfo;

FString UAuraProjectileSpell::GetDescription(int32 Level, UAbilityInfo* AbilityInfo, bool bIsCurLevel)
{
	if (AbilityInfo != nullptr)
	{
		// const FAuraAbilityInfo AbilityInformation = AbilityInfo->FindAbilityInfoForTagContainer(AbilityTags);
		// const FText FormattedAbilityDesc = FText::Format(AbilityInformation.AbilityDescText, FText::AsNumber(100), FText::AsNumber(100));
		const FText FormattedAbilityDesc = GetAbilityDesc(Level, AbilityInfo, bIsCurLevel);
		return FormattedAbilityDesc.ToString();
	}
	else
	{
		return Super::GetDescription(Level, AbilityInfo);
	}
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
	return Super::GetNextLevelDescription(Level);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePith, float PitchOverride)
{
	if (const bool bIsNotServer = !GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);;
	FTransform SpawnTransform;
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePith)
	{
		Rotation.Pitch = PitchOverride;
	}

	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		GetAvatarActorFromActorInfo());

	FGameplayEffectContextHandle GameplayEffectContextHandle = SourceASC->MakeEffectContext();
	GameplayEffectContextHandle.SetAbility(this);
	GameplayEffectContextHandle.AddSourceObject(Projectile);

	//创建这一次伤害的GE
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), GameplayEffectContextHandle);

	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	//通过蓝图上的伤害数值，设置当前GE的伤害元属性
	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			SpecHandle, Pair.Key, ScaledDamage);
	}


	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("火球伤害：%f"), ScaledDamage));

	//生成时候会一起生成需要造成伤害的GE，当此投射物对Actor进行重叠时候就进行Apply
	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTransform);
}

FText UAuraProjectileSpell::GetAbilityDesc(int32 Level, UAbilityInfo* AbilityInfo, bool bIsCurLevel)
{
	const FAuraAbilityInfo AbilityInformation = AbilityInfo->FindAbilityInfoForTagContainer(AbilityTags);
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	//火球
	if (AbilityInformation.AbilityTag.IsValid() && AbilityInformation.AbilityTag.MatchesTagExact(GameplayTags.Abilities_Fire_FireBolt))
	{
		if (const FProjectileSpellCommonRow1* RowConfig = AbilityInformation.AbilityConfig->FindRow<FProjectileSpellCommonRow1>(
			FName(*FString::FromInt(Level)), ""))
		{
			const FText FormattedAbilityDesc = FText::Format(bIsCurLevel ? AbilityInformation.AbilityDescText : AbilityInformation.AbilityNextLevelDescText,
			                                                 FText::AsNumber(RowConfig->MinDamage),
			                                                 FText::AsNumber(RowConfig->MaxDamage),
			                                                 FText::AsNumber(Level));
			return FormattedAbilityDesc;
		}
	}
	return FText();
}
