// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "IgniteGameplayEffect.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UIgniteGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	int FireStackDealDamageLimit = 1;
	
	

};
