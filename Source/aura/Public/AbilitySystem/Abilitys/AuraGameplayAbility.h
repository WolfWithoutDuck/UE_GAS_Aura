// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

class UAbilityInfo;
/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//默认激活的能力tag
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	virtual FString GetDescription(int32 Level, UAbilityInfo* AbilityInfo, bool bIsCurLevel = true);
	virtual FString GetNextLevelDescription(int32 Level);

	static FString GetLockDescription(int32 Level);
};
