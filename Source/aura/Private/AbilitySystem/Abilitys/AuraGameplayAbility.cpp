// 版权声明--


#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"

#include "AbilitySystem/Data/AbilityInfo.h"

FString UAuraGameplayAbility::GetDescription(int32 Level, UAbilityInfo* AbilityInfo, bool bIsCurLevel)
{
	return FString::Printf(TEXT("<Default>%s</><Level>%d</>"), L"默认技能名称 --- 空白 空白 空白 空白 空白 空白 空白 空白 空白 ", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s</><Level>%d</>"), L"默认技能名称 --- 空白 空白 空白 空白 空白 空白 空白 空白 空白 ", Level);
}

FString UAuraGameplayAbility::GetLockDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s</><Level>%d</>"), L"当前能力解锁等级： ", Level);
}
