// 版权声明--


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

/*
 *在创建初始化HUD 界面时候，创建HUD控制类
 *且对控制类进行初始化绑定各种属性 Tag变化事件分发
 */
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(
			this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(
			this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}
	return SpellMenuWidgetController;
}

UOverlayWidgetController* AAuraHUD::TryGetOverlayWidgetController()
{
	if (OverlayWidgetController != nullptr)
	{
		return OverlayWidgetController;
	}
	return nullptr;
}

UAttributeMenuWidgetController* AAuraHUD::TryGetAttributeMenuWidgetController()
{
	if (AttributeMenuWidgetController != nullptr)
	{
		return AttributeMenuWidgetController;
	}
	return nullptr;
}

USpellMenuWidgetController* AAuraHUD::TryGetSpellMenuWidgetController()
{
	if (SpellMenuWidgetController != nullptr)
	{
		return SpellMenuWidgetController;
	}
	return nullptr;
}

/*
 *角色初始化完成时候调用，初始化HUD界面 以及显示出来
 */
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
                           UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass 没有在BP_AuraHUD进行赋值，需要赋值才能初始化"))
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass 没有在BP_AuraHUD进行赋值，需要赋值才能初始化"))

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
