// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

class UAuraDamageGameplayAbility;
/**
 * 
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayTag, float> ExplosionDamage;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAuraDamageGameplayAbility> OwnerDamageAbility;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                             bool bFromSweep, const FHitResult& SweepResult) override;

	bool IsValidOverlap(AActor* OtherActor) const;


	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
};
