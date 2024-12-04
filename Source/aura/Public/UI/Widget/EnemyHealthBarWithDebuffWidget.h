// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "EnemyHealthBarWithDebuffWidget.generated.h"

class UHpBarDebuffWidget;
/**
 * 
 */
UCLASS()
class AURA_API UEnemyHealthBarWithDebuffWidget : public UAuraUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> DebuffItem;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> SlotParent;

	UFUNCTION(BlueprintCallable)
	void UpdateDebuffItem(UPARAM(ref)FActiveGameplayEffectHandle& ActiveGameplayEffectHandle);

	UFUNCTION(BlueprintCallable)
	void ClearDebuffItemList();

	UFUNCTION(BlueprintCallable)
	UHpBarDebuffWidget* GetDebuffWidget(FActiveGameplayEffectHandle& ActiveGameplayEffectHandle);

private:
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UHpBarDebuffWidget*> DebuffItemList;
};
