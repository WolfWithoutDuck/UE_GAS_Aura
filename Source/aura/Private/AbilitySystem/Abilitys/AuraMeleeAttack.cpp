// 版权声明--


#include "AbilitySystem/Abilitys/AuraMeleeAttack.h"

#include "AuraGameplayTags.h"


TMap<FGameplayTag, float> UAuraMeleeAttack::GetCauseDamage()
{
	//临时处理，后面改成读表
	TMap<FGameplayTag, float> Damage;
	Damage.Add(FAuraGameplayTags::Get().Damage_Physical, 20.f);
	return Damage;
}
