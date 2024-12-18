// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "OverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

struct FAuraGameplayTags;
//技能Widget UI选中广播
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellTreeChangedSelectSignature, UAuraUserWidget*, SpellItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointButtonEnable, bool, bEquipButtonEnable, FString, DescStr, FString,
                                              NextLevelDescStr);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FSpellTreeChangedSelectSignature SpellTreeChangedSelectDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;


	UFUNCTION(BlueprintCallable)
	void SkillTreeWidgetSelected(UAuraUserWidget* SpellItem) const;

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void GlobeDeselected();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	//主动技能树  和 被动技能 点击
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	//监听服务器广播能力装备
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

private:
	static void ShouldEnableButton(const FGameplayTag& AbilityStatus, int32 SpellPoint, bool& bShouldEnableSpellPointButton, bool& bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = {FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSpellPoints = 0;
	bool bWaitingForEquipSelection = false;
	FGameplayTag SelectedSlot;
};
