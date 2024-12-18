// 版权声明--


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilitiesInputActionForTag(const FGameplayTag& InputTag,
                                                                     bool bLogNotFound) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("不能根据GameTag，找到能力输入Action [%s],在 InputConfig[%s]"), *InputTag.ToString(),
		       *GetNameSafe(this))
	}
	return nullptr;
}
