// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="应用的效果")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="应用的效果")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
};
