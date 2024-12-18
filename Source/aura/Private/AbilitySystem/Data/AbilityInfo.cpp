// 版权声明--


#include "AbilitySystem/Data/AbilityInfo.h"

#include "aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT(",在类[%s]中，找不到AbilityTag的信息[%s]"), *GetNameSafe(this), *AbilityTag.ToString())
	}

	return FAuraAbilityInfo();
}

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTagContainer(const FGameplayTagContainer& AbilityTags, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (AbilityTags.HasTag(Info.AbilityTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT(",在类[%s]中，找不到AbilityTag的信息"), *GetNameSafe(this))
	}

	return FAuraAbilityInfo();
}
