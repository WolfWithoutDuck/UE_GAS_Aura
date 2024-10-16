// 版权声明--


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const auto& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("没有找到当前Tag信息[%s] 在AttributeInfo [%s]中"), *AttributeTag.ToString(),
		       *GetNameSafe(this));
	}
	return FAuraAttributeInfo();
}
