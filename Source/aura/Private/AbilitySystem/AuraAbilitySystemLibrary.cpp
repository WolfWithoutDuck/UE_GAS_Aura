// 版权声明--


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "Interation/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams OutWCParams;
	if (AAuraHUD* OutAuraHud = nullptr; MakeWidgetControllerParams(WorldContextObject, OutWCParams, OutAuraHud))
	{
		if (OutAuraHud->TryGetSpellMenuWidgetController())
		{
			return OutAuraHud->TryGetOverlayWidgetController();
		}
		return OutAuraHud->GetOverlayWidgetController(OutWCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams OutWCParams;
	if (AAuraHUD* OutAuraHud = nullptr; MakeWidgetControllerParams(WorldContextObject, OutWCParams, OutAuraHud))
	{
		if (OutAuraHud->TryGetSpellMenuWidgetController())
		{
			return OutAuraHud->TryGetAttributeMenuWidgetController();
		}
		return OutAuraHud->GetAttributeMenuWidgetController(OutWCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams OutWCParams;
	if (AAuraHUD* OutAuraHud = nullptr; MakeWidgetControllerParams(WorldContextObject, OutWCParams, OutAuraHud))
	{
		if (OutAuraHud->TryGetSpellMenuWidgetController())
		{
			return OutAuraHud->TryGetSpellMenuWidgetController();
		}
		return OutAuraHud->GetSpellMenuWidgetController(OutWCParams);
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHud)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHud = Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHud)
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.AttributeSet = AS;
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;

			return true;
		}
	}
	return false;
}

/*
 *	根据传入进来的怪物类型
 *	初始化怪物的默认属性
 */
void UAuraAbilitySystemLibrary::InitializedDefaultAttributes(const UObject* WorldContextObject,
                                                             ECharacterClass CharacterClassType, float Level,
                                                             UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);;
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClassType);

	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributes, Level, PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributes, Level, SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes, Level, VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

/*
 * 根据传入的角色类型枚举，初始化默认的角色能力和每个角色自己的角色能力，在EnemyBegin中初始化
 */
void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClassType)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
		return;

	for (const auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClassType);
	for (auto AbilityClass : DefaultInfo.StartupAbilities)
	{
		// if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

/**
 * 根据等级获取怪物经验信息
 * @param WorldContextObject 世界上下文
 * @param CharacterClassType 角色（怪物）类型枚举
 * @param CharacterLevel 角色（怪物）等级
 * @return 
 */
int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClassType, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
		return 0;
	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClassType);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);
	return static_cast<int32>(XPReward);
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	//GameMode 只能在服务端获取 记住
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr)
		return nullptr;
	return AuraGameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	//GameMode 只能在服务端获取 记住
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr)
		return nullptr;
	return AuraGameMode->AbilityInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		AuraEffectContext->SetCriticalHit(bInIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
                                                           const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
		                                FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                                FCollisionShape::MakeSphere(Radius), SphereParams);

		for (FOverlapResult& Overlap : Overlaps)
		{
			if (const bool ImplementsCombatInterface = Overlap.GetActor()->Implements<UCombatInterface>())
			{
				if (const bool IsAlive = !ICombatInterface::Execute_IsDeath(Overlap.GetActor()))
				{
					OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
				}
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bFirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool bSecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));
	const bool bFirstIsEnemy = FirstActor->ActorHasTag(FName("Enemy"));
	const bool bSecondIsEnemy = SecondActor->ActorHasTag(FName("Enemy"));

	const bool bBothArePlayers = bFirstIsPlayer && bSecondIsPlayer;
	const bool bBothAreEnemy = bFirstIsEnemy && bSecondIsEnemy;
	const bool bFriends = bBothArePlayers || bBothAreEnemy;
	return !bFriends;
}

FGameplayTagContainer UAuraAbilitySystemLibrary::GetSetByCallerModifiersTags(const TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// const TArray<FGameplayModifierInfo> Modifiers = SpecHandle.Data.Get()->Def->Modifiers;
	const TArray<FGameplayModifierInfo> Modifiers = GameplayEffectClass->GetDefaultObject<UGameplayEffect>()->Modifiers;
	if (Modifiers.Num() > 0)
	{
		FGameplayTagContainer TagForSetByCaller;
		for (auto Modifier : Modifiers)
		{
			if (Modifier.ModifierMagnitude.GetSetByCallerFloat().DataTag.IsValid())
			{
				TagForSetByCaller.AddTag(Modifier.ModifierMagnitude.GetSetByCallerFloat().DataTag);
			}
		}
		return TagForSetByCaller;
	}
	return FGameplayTagContainer();
}
