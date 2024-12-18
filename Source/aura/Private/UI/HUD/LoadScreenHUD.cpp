// 版权声明--


#include "UI/HUD/LoadScreenHUD.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	//先实例化主界面的Model类
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	//再实例化小控件的Model类
	LoadScreenViewModel->InitializeLoadSlot();

	//再加载主界面UI，然后加载小控件
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	LoadScreenWidget->BlueprintInitializeWidget();
	LoadScreenViewModel->LoadData();
}
