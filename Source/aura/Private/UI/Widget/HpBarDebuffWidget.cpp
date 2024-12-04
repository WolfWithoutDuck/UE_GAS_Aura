// 版权声明--


#include "UI/Widget/HpBarDebuffWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/TextBlock.h"


void UHpBarDebuffWidget::UpdateDebuffWidget(FActiveGameplayEffectHandle& ActiveGameplayEffectHandle)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(DebuffTimerHandle))
	{
		// 如果定时器正在运行，清除该定时器，停止其计时
		GetWorld()->GetTimerManager().ClearTimer(DebuffTimerHandle);
	}

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const FGameplayTagContainer DebuffTags = UAuraAbilitySystemLibrary::GetActiveGameplayEffectDebuffTag(ActiveGameplayEffectHandle);;
	const FGameplayTag ShowDebuffTag = DebuffTags.First();
	if (ShowDebuffTag.IsValid())
	{
		if (GameplayTags.DebuffToShowString.Contains(ShowDebuffTag))
		{
			DebuffName->SetText((GameplayTags.DebuffToShowString[ShowDebuffTag]));
		}
		UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectRemainingDuration(this, ActiveGameplayEffectHandle);

		CurActiveGameplayEffectHandle = ActiveGameplayEffectHandle;
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UHpBarDebuffWidget::UpdateDebuffTimerText);
		GetWorld()->GetTimerManager().SetTimer(DebuffTimerHandle, Delegate, 1.f, true);
		const int32 StackCount = UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectStackCount(ActiveGameplayEffectHandle);
		DebuffStackCount->SetText(FText::AsNumber(StackCount));
		UpdateDebuffTimerText();
	}
}

void UHpBarDebuffWidget::UpdateDebuffTimerText()
{
	if (CurActiveGameplayEffectHandle.IsValid())
	{
		const float RemainingTime = UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectRemainingDuration(this, CurActiveGameplayEffectHandle);
		const FString TimeText = FString::FromInt(FMath::FloorToInt(RemainingTime));
		DebuffRemainingTime->SetText(FText::FromString(TimeText + "s"));
	
		if (RemainingTime <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(DebuffTimerHandle);
			RemoveFromParent();
		}
	}
}

bool UHpBarDebuffWidget::ItemIsValid() const
{
	if (GetWorld()->GetTimerManager().IsTimerActive(DebuffTimerHandle))
	{
		return true;
	}
	return false;
}
