// 版权声明--


#include "UI/WidgetController/ModifiersMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeModifierDesc.h"


void UModifiersMenuWidgetController::BindCallbacksToDependencies()
{
	//绑定当前界面上的属性
	for (auto& Pair : AttributeModifierDescClass->AttributeModifierDescriptions)
	{
		GetAuraASC()->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
			{
				//属性变化通知
				AttributeChange.Broadcast(Pair.Key);
			}
		);
	}
}

FText UModifiersMenuWidgetController::GetDefaultAttributeText(const FGameplayAttribute& Attribute) const
{
	if (FGameModifierDesc* Desc = AttributeModifierDescClass->AttributeModifierDescriptions.Find(Attribute))
	{
		return Desc->DescAdd;
	}
	return FText();
}
