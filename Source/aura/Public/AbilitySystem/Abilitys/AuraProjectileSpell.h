// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
struct FGameplayTag;

/**
 *通用投射物表格行
 */
USTRUCT(BlueprintType)
struct FProjectileSpellCommonRow1 : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ManaCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxDamage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MinDamage = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseRadius = 0;
};

/**
 * 投射物能力父类
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;


	UFUNCTION(BlueprintCallable, Category="投射物")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePith = false, float PitchOverride = 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NumProjectiles = 5;


};
