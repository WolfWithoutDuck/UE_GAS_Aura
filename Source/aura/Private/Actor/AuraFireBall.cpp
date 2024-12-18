// 版权声明--


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor))
		return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			//计算死亡击飞角度
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

bool AAuraFireBall::IsValidOverlap(AActor* OtherActor) const
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr)
		return false;

	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	//碰撞到自己，直接return
	if (SourceAvatarActor == OtherActor)
		return false;

	//碰撞到友军也直接return
	if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor))
		return false;

	return true;
}
