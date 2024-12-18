// 版权声明--


#include "AbilitySystem/Abilitys/AuraFireBlast.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(FireBallClass, SpawnTransform, GetOwningActorFromActorInfo(),
		                                                                        CurrentActorInfo->PlayerController->GetPawn(),
		                                                                        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		FireBall->ExplosionDamage = GetExplosionDamage();
		FireBall->OwnerDamageAbility = this;
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}
	return FireBalls;
}

TMap<FGameplayTag, float> UAuraFireBlast::GetCauseDamage()
{
	TMap<FGameplayTag, float> Damage;
	Damage.Add(FAuraGameplayTags::Get().Damage_Fire, 8.f);
	return Damage;
}

//火球爆炸伤害
TMap<FGameplayTag, float> UAuraFireBlast::GetExplosionDamage()
{
	TMap<FGameplayTag, float> Damage;
	Damage.Add(FAuraGameplayTags::Get().Damage_Fire, 5.f);
	return Damage;
}
