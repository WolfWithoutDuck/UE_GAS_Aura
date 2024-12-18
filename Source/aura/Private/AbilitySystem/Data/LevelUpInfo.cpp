// 版权声明--


#include "AbilitySystem/Data/LevelUpInfo.h"

/*
 *根据传进来的经验，获取当前能升到几级
 */
int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpInformation.Num() - 1 <= Level)
		{
			return Level;
		}

		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
