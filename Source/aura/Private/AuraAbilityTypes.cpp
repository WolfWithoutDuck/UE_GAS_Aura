#include "AuraAbilityTypes.h"

#include "aura/AuraLogChannels.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 14;
		}
		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 15;
		}
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 16;
			if (RadialDamageInnerRadius > 0.f)
			{
				RepBits |= 1 << 17;
			}
			if (RadialDamageOuterRadius > 0.f)
			{
				RepBits |= 1 << 18;
			}
			if (!RadialDamageOrigin.IsZero())
			{
				RepBits |= 1 << 19;
			}
		}
		if (bIsSuccessfulIgnite)
		{
			RepBits |= 1 << 20;
		}
		if (IgniteDamage > 0.f)
		{
			RepBits |= 1 << 21;
		}
		if (IgniteDuration > 0.f)
		{
			RepBits |= 1 << 22;
		}
		if (!FMath::IsNearlyZero(IgnitePeriod - 1.f))
		{
			RepBits |= 1 << 23;
		}
	}


	Ar.SerializeBits(&RepBits, 23);

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}

	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 10))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 16))
	{
		Ar << bIsRadialDamage;

		if (RepBits & (1 << 17))
		{
			Ar << RadialDamageInnerRadius;
		}
		if (RepBits & (1 << 18))
		{
			Ar << RadialDamageOuterRadius;
		}
		if (RepBits & (1 << 19))
		{
			RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
		}
	}
	if (RepBits & (1 << 20))
	{
		Ar << bIsSuccessfulIgnite;
	}
	if (RepBits & (1 << 21))
	{
		Ar << IgniteDamage;
	}
	if (RepBits & (1 << 22))
	{
		Ar << IgniteDuration;
	}
	if (RepBits & (1 << 23))
	{
		Ar << IgnitePeriod;
	}


	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;

	return true;
}

void FAuraGameplayEffectContext::SetIgniteStackInfo(float InIgniteDamage, float IgniteEndTime)
{
	IgniteDamageToEndTime.Add(IgniteEndTime, InIgniteDamage);
}

TMap<float, float>& FAuraGameplayEffectContext::GetIgniteCauseDamage()
{
	return IgniteDamageToEndTime;
}

void FAuraGameplayEffectContext::CleanUpExpiredEffects(float CurrentTime)
{
	for (auto It = IgniteDamageToEndTime.CreateIterator(); It; ++It)
	{
		if (It.Key() < CurrentTime) // 检查结束时间
		{
			It.RemoveCurrent(); // 移除过期的点燃伤害
		}
	}
}

float FAuraGameplayEffectContext::GetTotalIgniteDamage(float CurrentTime, int32 IgniteStack)
{
	CleanUpExpiredEffects(CurrentTime); // 清理过期的效果
	TArray<float> ValidDamages;

	UE_LOG(LogAura, Warning, TEXT("造成点燃伤害 -->点燃堆栈->[%d] "), IgniteDamageToEndTime.Num());
	// 收集有效的伤害
	for (const auto& Pair : IgniteDamageToEndTime)
	{
		ValidDamages.Add(Pair.Value);
	}

	// 如果没有有效的伤害，返回 0
	if (ValidDamages.Num() == 0)
	{
		return 0.f;
	}

	// 排序有效伤害
	ValidDamages.Sort([](const float& A, const float& B)
	{
		return A > B; // 降序排序
	});

	// 计算总伤害，最多累加 IgniteStack 次
	float TotalDamage = 0.f;
	for (int32 i = 0; i < FMath::Min(IgniteStack, ValidDamages.Num()); i++)
	{
		TotalDamage += ValidDamages[i]; // 累加最高的伤害
	}

	return TotalDamage; // 返回总伤害
}

void FAuraGameplayEffectContext::InitIgniteStackInfo(const TMap<float, float>& IgniteDamageStack)
{
	IgniteDamageToEndTime.Empty();
	IgniteDamageToEndTime = IgniteDamageStack;
}
