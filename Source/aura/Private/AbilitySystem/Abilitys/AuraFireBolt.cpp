// 版权声明--


#include "AbilitySystem/Abilitys/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Actor/AuraProjectile.h"
#include "aura/AuraLogChannels.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interation/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level, UAbilityInfo* AbilityInfo, bool bIsCurLevel)
{
	if (AbilityInfo != nullptr)
	{
		const float ManaCost = GetManaCost(Level, AbilityInfo);
		const FAuraAbilityInfo AbilityInformation = AbilityInfo->FindAbilityInfoForTagContainer(AbilityTags);
		if (const FProjectileSpellCommonRow1* RowConfig = AbilityInformation.AbilityConfig->FindRow<FProjectileSpellCommonRow1>(
			FName(*FString::FromInt(Level)), ""))
		{
			const FText FormattedAbilityDesc = FText::Format(bIsCurLevel ? AbilityInformation.AbilityDescText : AbilityInformation.AbilityNextLevelDescText,
			                                                 FText::AsNumber(RowConfig->MinDamage),
			                                                 FText::AsNumber(RowConfig->MaxDamage),
			                                                 FText::AsNumber(Level),
			                                                 FText::AsNumber(ManaCost));
			return FormattedAbilityDesc.ToString();
		}
	}

	return Super::GetDescription(Level, AbilityInfo);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	return Super::GetNextLevelDescription(Level);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePith, float PitchOverride,
                                     AActor* HomingTarget)
{
	if (const bool bIsNotServer = !GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);;
	// FTransform SpawnTransform;
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePith)
		Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectile = FMath::Min(NumProjectiles, GetAbilityLevel());
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectilesSpread, EffectiveNumProjectile);

	for (const FRotator& Rot : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
		                                                                              Cast<APawn>(GetOwningActorFromActorInfo()),
		                                                                              ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}

		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;


		Projectile->FinishSpawning(SpawnTransform);
	}
}

/**
 * 根据Ability能力读表，一个Ability代表一个技能
 * TODO ： 怪物使用这个Ability的话，那么怪物也会用这个表，看后面是否需要区分
 * @return 
 */
TMap<FGameplayTag, float> UAuraFireBolt::GetCauseDamage()
{
	TMap<FGameplayTag, float> DamageInfo;
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	if (const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActorFromActorInfo()))
	{
		const FAuraAbilityInfo AbilityInformation = AbilityInfo->FindAbilityInfoForTagContainer(AbilityTags);
		if (const FProjectileSpellCommonRow1* RowConfig = AbilityInformation.AbilityConfig->FindRow<FProjectileSpellCommonRow1>(
			FName(*FString::FromInt(GetAbilityLevel())), ""))
		{
			//根据当前等级读表，获取最大最小值的随机值，默认为火焰伤害
			const int32 RandomDamage = FMath::RandRange(RowConfig->MinDamage, RowConfig->MaxDamage);
			// UE_LOG(LogAura, Error, TEXT("随机伤害为[%d-%d] %d"), RowConfig->MinDamage, RowConfig->MaxDamage, RandomDamage);
			DamageInfo.Add(GameplayTags.Damage_Fire, RandomDamage);
		}
		return DamageInfo;
	}
	return DamageInfo;
}

float UAuraFireBolt::GetManaCost(float InLevel, const UAbilityInfo* AbilityInfo)
{
	if (AbilityInfo)
	{
		const FAuraAbilityInfo AbilityInformation = AbilityInfo->FindAbilityInfoForTagContainer(AbilityTags);
		if (const FProjectileSpellCommonRow1* RowConfig = AbilityInformation.AbilityConfig->FindRow<FProjectileSpellCommonRow1>(
			FName(*FString::FromInt(InLevel)), ""))
		{
			return RowConfig->ManaCost;
		}
	}
	return 0.f;
}

float UAuraFireBolt::GetCoolDown(float InLevel, UAbilityInfo* AbilityInfo)
{
	//TODO 使用自定义CoolDown  ExecuteGE  
	return 0.f;
}
