// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameplayModifiers.generated.h"

//属性定义
USTRUCT(BlueprintType)
struct FGameModifierDefine
{
	GENERATED_BODY()
	//属性
	UPROPERTY(EditDefaultsOnly)
	FGameplayAttribute Attribute;

	/**属性操作 The numeric operation of this modifier: Override, Add, Multiply, etc  */
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;

	//属性数值，默认使用ScalableFloatMagnitude
	UPROPERTY(EditDefaultsOnly)
	float ModifierMagnitude;
};

//GameplayEffect 修改定义
USTRUCT(BlueprintType)
struct FGameModifierInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameModifierDefine> GameModifiers;

	//当前GE 标记属于什么类别，用于获取
	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer MarkTag;
};


/**
 * 局内属性修改
 */
UCLASS()
class AURA_API UGameplayModifiers : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="修改词条")
	TArray<FGameModifierInfo> FGameModifiers;

	UFUNCTION(BlueprintCallable)
	TArray<FGameModifierInfo> GetGameModifiers();
};
