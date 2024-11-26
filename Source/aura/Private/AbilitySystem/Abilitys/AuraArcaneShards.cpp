// 版权声明--


#include "AbilitySystem/Abilitys/AuraArcaneShards.h"

#include "AuraGameplayTags.h"

TMap<FGameplayTag, float> UAuraArcaneShards::GetCauseDamage()
{
	TMap<FGameplayTag, float> Damage;
	Damage.Add(FAuraGameplayTags::Get().Damage_Arcane, 10.f);
	return Damage;
}
