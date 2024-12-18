// 版权声明--


#include "AbilitySystem/Abilitys/AuraSummonAbility.h"


TArray<FVector> UAuraSummonAbility::GetSpawnLocation()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	//向前向量向左旋转角度的一半
	const FVector LeftOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	//向前向量向右旋转角度的一半
	const FVector RightOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);

	// //绘制扇形左边的线
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance,
	//                                      4.f, FLinearColor::Green, 3.f);
	// //绘制扇形右边的线
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance,
	//                                      4.f, FLinearColor::Green, 3.f);

	//扇形内平均绘制球体
	TArray<FVector> SpawnLocations;
	const float DeltaSpread = SpawnSpread / NumMinions;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(i * -DeltaSpread - NumMinions / 2, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 500.f), ChosenSpawnLocation - FVector(0.f, 0.f, 500.f),
		                                     ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenSpawnLocation);

		// DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.F, 12, FColor::Orange, false, 3.f);
		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance,
		//                                      4.f, FLinearColor::Green, 3.f);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
