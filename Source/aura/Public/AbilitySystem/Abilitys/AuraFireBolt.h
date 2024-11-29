// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 火球，技能伤害千变万化，不可能在Damage父类中各种方法，每个Ability自己定义自己的获取伤害的方法，多段伤害的Ability就定义多段
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level, UAbilityInfo* AbilityInfo, bool bIsCurLevel = true) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePith, float PitchOverride,
	                      AActor* HomingTarget);

protected:
	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float ProjectilesSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category="FireBolt")
	bool bLaunchHomingProjectiles = true;

	virtual TMap<FGameplayTag, float> GetCauseDamage() override;
	virtual TMap<FGameplayTag, float> GetBaseAndAddedDamage() override;
	virtual float GetIgniteChance() override;
	virtual float GetIgniteDuration() override;
	virtual TMap<FGameplayTag, float> GetIgniteDamage() override;

	float GetManaCost(float InLevel, const UAbilityInfo* AbilityInfo);
	float GetCoolDown(float InLevel, UAbilityInfo* AbilityInfo);
};
