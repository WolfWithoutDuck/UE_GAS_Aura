// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Character/AuraCharacterBase.h"
#include "Interation/EnemyInterface.h"
#include "Interation/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
enum class ECharacterClass : uint8;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	virtual void PossessedBy(AController* NewController) override;

	//敌人接口
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	virtual void Die(const FVector& DeathImpulse) override;

	//combat接口
	virtual int32 GetPlayerLevel_Implementation();
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category= "战斗")
	bool bHitReacting = false;


	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "战斗")
	float LifeSpan = 5.f;

	//在行为树节点中，触发攻击节点时候，设置战斗的目标（攻击对象）
	UPROPERTY(BlueprintReadWrite, Category="战斗")
	TObjectPtr<AActor> CombatTarget;

	void SetLevel(const int32 InLevel) { Level = InLevel; }

protected:
	//生命周期
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttribute() const override;
	virtual void StunTagChange(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
};
