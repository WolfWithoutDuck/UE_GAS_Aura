// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.h"
#include "MVVM_LoadScreen.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSelectedSlot, int32, SeletedSlot);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	void InitializeLoadSlot();

	UPROPERTY(BlueprintAssignable)
	FUpdateSelectedSlot UpdateSelectedSlot;

	UFUNCTION
	(BlueprintCallable)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	//选择新存档按钮按下
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

	//新游戏按下
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	//选中存档按下
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	//删除存档按钮按下
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed(int32 Slot);

	void LoadData();


	void SetBindToCreateTemp(FText InBindToCreateTemp);
	FText GetBindToCreateTemp() const { return BindToCreateTemp; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BindToCreateTemp;

private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
	UMVVM_LoadSlot* SelectedSlot;
};
