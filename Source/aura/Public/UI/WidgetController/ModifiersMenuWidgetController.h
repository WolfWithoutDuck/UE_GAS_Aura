// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "ModifiersMenuWidgetController.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangeSignature, const FGameplayAttribute&, Attribute);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UModifiersMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="词条修改")
	TObjectPtr<UAttributeModifierDesc> AttributeModifierDescClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="词条修改")
	TObjectPtr<UGameplayModifiers> GameplayModifiersClass;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributs")
	FAttributeChangeSignature AttributeChange;

	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	FText GetDefaultAttributeText(const FGameplayAttribute& Attribute) const;
};
