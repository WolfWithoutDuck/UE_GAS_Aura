// 版权声明--


#include "..\..\..\Public\AbilitySystem\Data\AttributeModifierDesc.h"


FText UAttributeModifierDesc::GetAttributeModifierDesc(FGameModifierDefine& GameModifierDefine)
{
	FGameModifierDesc GameModifierDesc;
	const bool bGetDesc = GetGameModifierDesc(GameModifierDefine.Attribute, GameModifierDesc);
	if (bGetDesc)
	{
		const FText ShowDesc = GameModifierDefine.ModifierMagnitude >= 0 ? GameModifierDesc.DescAdd : GameModifierDesc.DescReduce;
		return FText::Format(ShowDesc, FMath::Abs(GameModifierDefine.ModifierMagnitude));
	}
	return FText();
}

bool UAttributeModifierDesc::GetGameModifierDesc(const FGameplayAttribute& Attribute, FGameModifierDesc& GameModifierDesc)
{
	if (AttributeModifierDescriptions.Contains(Attribute))
	{
		GameModifierDesc = AttributeModifierDescriptions[Attribute];
		return true;
	}
	return false;
}


FText UAttributeModifierDesc::GetGameModifierDesc(FGameModifierInfo& ModifierInfo)
{
	if (ModifierInfo.GameModifiers.Num() == 0)
	{
		return FText();
	}
	FText ModifierDesc;
	if (ModifierInfo.GameModifiers.Num() > 0)
	{
		for (FGameModifierDefine& SingleAttributeDefine : ModifierInfo.GameModifiers)
		{
			FText formatText = FText::FromString("{0}\n{1}");
			if (ModifierDesc.IsEmpty())
			{
				ModifierDesc = GetAttributeModifierDesc(SingleAttributeDefine);
			}
			else
			{
				ModifierDesc = FText::Format(formatText, ModifierDesc, GetAttributeModifierDesc(SingleAttributeDefine));
			}
		}
	}
	return ModifierDesc;
}
