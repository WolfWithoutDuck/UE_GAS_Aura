// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AuraEnemySpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Interation/SaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:
	AAuraEnemySpawnVolume();


	virtual void LoadActor_Implementation() override;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
};
