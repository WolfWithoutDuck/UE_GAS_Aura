// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayModifiers.h"
#include "Engine/DataAsset.h"
#include "AttributeModifierDesc.generated.h"

//单条属性修改描述
USTRUCT(BlueprintType)
struct FGameModifierDesc
{
	GENERATED_BODY()

	//属性 比如 
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute AttributeDefine;

	//增加的文本 比如 点燃几率增加{0}%
	UPROPERTY(EditDefaultsOnly)
	FText DescAdd;

	//减少的文本 比如 点燃几率减少{0}%
	UPROPERTY(EditDefaultsOnly)
	FText DescReduce;
};


/**
 * 单条属性描述
 */
UCLASS()
class AURA_API UAttributeModifierDesc : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayAttribute, FGameModifierDesc> AttributeModifierDescriptions;

	//获取单条属性描述
	FText GetAttributeModifierDesc(FGameModifierDefine& GameModifierDefine);

	bool GetGameModifierDesc(const FGameplayAttribute& Attribute, FGameModifierDesc& GameModifierDesc);

	//获取整块的多条属性整合描述
	UFUNCTION(BlueprintCallable)
	FText GetGameModifierDesc(UPARAM(ref) FGameModifierInfo& ModifierInfo);
};
