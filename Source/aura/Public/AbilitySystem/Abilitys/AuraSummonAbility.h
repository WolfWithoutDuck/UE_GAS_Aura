// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilitys/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocation();

	UPROPERTY(EditAnywhere, Category="召唤")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category="召唤")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category="召唤")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category="召唤")
	float MaxSpawnDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category="召唤")
	float SpawnSpread = 90.f;

	UFUNCTION(BlueprintPure, Category="召唤")
	TSubclassOf<APawn> GetRandomMinionClass();
};
