// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_IgniteDamage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_IgniteDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	int32 TestCount;
	UMMC_IgniteDamage();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
