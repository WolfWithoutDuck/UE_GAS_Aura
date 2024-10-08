// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interation/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	//敌人接口
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	//生命周期
	virtual void BeginPlay() override;
};
