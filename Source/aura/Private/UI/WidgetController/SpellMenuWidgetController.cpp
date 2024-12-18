// 版权声明--


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			//当状态发生变化，更新选中的Item状态
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;
				ShouldEnableButton(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
				FString Desc;
				FString NextLevelDesc;
				GetAuraASC()->GetDescByAbilityTag(AbilityTag, Desc, NextLevelDesc);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Desc, NextLevelDesc);
			}


			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
		{
			SpellPointChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
			CurrentSpellPoints = SpellPoints;

			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButton(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);

			FString Desc;
			FString NextLevelDesc;
			GetAuraASC()->GetDescByAbilityTag(SelectedAbility.Ability, Desc, NextLevelDesc);

			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Desc, NextLevelDesc);
		});

	GetAuraASC()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
}

void USpellMenuWidgetController::SkillTreeWidgetSelected(UAuraUserWidget* SpellItem) const
{
	SpellTreeChangedSelectDelegate.Broadcast(SpellItem);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}


	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;


	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButton(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);

	FString Desc;
	FString NextLevelDesc;
	GetAuraASC()->GetDescByAbilityTag(SelectedAbility.Ability, Desc, NextLevelDesc);

	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Desc, NextLevelDesc);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselected()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}


	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
	WaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	//根据选中的AbilityTag  记录获取当前Ability的InputTag
	const FGameplayTag SelectionStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectionStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

/**
 * 插槽按钮点击，装备技能
 * @param SlotTag 
 * @param AbilityType 
 */
void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection)
		return;
	//检查对应的技能类型是否匹配
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType))
		return;

	GetAuraASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot,
                                                   const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag);
}

/**
 * 状态改变事件广播时候调用
 * @param AbilityStatus 
 * @param SpellPoint 
 * @param bShouldEnableSpellPointButton 
 * @param bShouldEnableEquipButton 
 */
void USpellMenuWidgetController::ShouldEnableButton(const FGameplayTag& AbilityStatus, int32 SpellPoint, bool& bShouldEnableSpellPointButton,
                                                    bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bShouldEnableSpellPointButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpellPointButton = SpellPoint > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableEquipButton = false;
		bShouldEnableSpellPointButton = SpellPoint > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpellPointButton = SpellPoint > 0;
	}
}
