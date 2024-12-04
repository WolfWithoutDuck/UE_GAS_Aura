// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Blueprint/UserWidget.h"
#include "HpBarDebuffWidget.generated.h"

class UTextBlock;
/**
 * 怪物血条下的Debuff小图标
 */
UCLASS()
class AURA_API UHpBarDebuffWidget : public UUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> DebuffName;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> DebuffRemainingTime;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> DebuffStackCount;

	UFUNCTION(BlueprintCallable)
	void UpdateDebuffWidget(FActiveGameplayEffectHandle& ActiveGameplayEffectHandle);

	FTimerHandle DebuffTimerHandle;

	void UpdateDebuffTimerText();

	bool ItemIsValid() const;

private:
	FActiveGameplayEffectHandle CurActiveGameplayEffectHandle;
};
