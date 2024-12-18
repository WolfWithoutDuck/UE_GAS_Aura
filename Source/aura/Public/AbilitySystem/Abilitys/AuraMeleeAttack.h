// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraDamageGameplayAbility.h"
#include "AuraMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraMeleeAttack : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual TMap<FGameplayTag, float> GetCauseDamage() override;
};
