// 版权声明--


#include "UI/ViewModel/MVVM_LoadSlot.h"

#include "Kismet/GameplayStatics.h"

void UMVVM_LoadSlot::InitializedSlot()
{
	//根据当前存档数据的枚举，设置当前显示的内容
	const int32 WidgetSwitchIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitchIndex);
}

void UMVVM_LoadSlot::SetAutoSetToBind(FString InAutoSetToBind)
{
	UE_MVVM_SET_PROPERTY_VALUE(AutoSetToBind, InAutoSetToBind);
}

void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

void UMVVM_LoadSlot::SetPlayerLevel(INT32 InPlayerLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InPlayerLevel);
}
