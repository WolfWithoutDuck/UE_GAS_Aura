// 版权声明--


#include "AbilitySystem/MMC/MMC_IgniteDamage.h"

#include "AuraAbilityTypes.h"


UMMC_IgniteDamage::UMMC_IgniteDamage()
{
}

float UMMC_IgniteDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Damage = 0;
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Spec.GetContext().Get()))
	{
		Damage = AuraEffectContext->GetTotalIgniteDamage(AuraEffectContext->GetSourceObject()->GetWorld()->GetTimeSeconds(), 2);
	}
	return Damage;
}
