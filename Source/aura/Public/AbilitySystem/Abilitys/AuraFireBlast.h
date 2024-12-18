// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="FireBlast")
	int32 NumFireBalls = 12;


	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();

	UFUNCTION(BlueprintCallable)
	virtual TMap<FGameplayTag, float> GetCauseDamage() override;

	
	UFUNCTION(BlueprintCallable)
	TMap<FGameplayTag, float> GetExplosionDamage();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireBall> FireBallClass;
};
