// 版权声明--


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlot()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlot_0->SetAutoSetToBind(FString("Bind000"));
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlot_1->SetAutoSetToBind(FString("Bind001"));
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;
	LoadSlot_2->SetAutoSetToBind(FString("Bind002"));
	LoadSlots.Add(2, LoadSlot_2);

	SetBindToCreateTemp(FText::FromString(TEXT("测试绑定111")));
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SetPlayerLevel(1);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	LoadSlots[Slot]->MapAssetName = AuraGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();

	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializedSlot();

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	AuraGameInstance->LoadSlotName = LoadSlots[Slot]->LoadSlotName;
	AuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	UpdateSelectedSlot.Broadcast(Slot);
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->SetPlayerName(FString());
		UpdateSelectedSlot.Broadcast(-1);
		SelectedSlot->InitializedSlot();
	}
}

void UMVVM_LoadScreen::PlayButtonPressed(int32 Slot)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());

	AuraGameInstance->PlayerStartTag = LoadSlots[Slot]->PlayerStartTag;
	AuraGameInstance->LoadSlotName = LoadSlots[Slot]->LoadSlotName;
	AuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;


	AuraGameMode->TravelToMap(LoadSlots[Slot]);
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameMode))
	{
		return;
	}

	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->LoadSlotName, LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		const FString MapName = SaveObject->MapName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetMapName(MapName);
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
		LoadSlot.Value->InitializedSlot();
	}
}

void UMVVM_LoadScreen::SetBindToCreateTemp(FText InBindToCreateTemp)
{
	UE_MVVM_SET_PROPERTY_VALUE(BindToCreateTemp, InBindToCreateTemp);
}
