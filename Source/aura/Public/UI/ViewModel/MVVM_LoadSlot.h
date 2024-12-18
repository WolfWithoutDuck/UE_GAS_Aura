// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	void InitializedSlot();


	UPROPERTY()
	FString LoadSlotName;

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	FString MapAssetName;

	//用来绑定生成model模型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString AutoSetToBind;

	void SetAutoSetToBind(FString InAutoSetToBind);
	FString GetAutoSetToBind() const { return AutoSetToBind; }


	void SetPlayerName(FString InPlayerName);
	FString GetPlayerName() const { return PlayerName; }

	void SetMapName(FString InMapName);
	FString GetMapName() const { return MapName; }

	void SetPlayerLevel(int32 InPlayerLevel);
	int32 GetPlayerLevel() const { return PlayerLevel; };

private:
	//需要进行绑定通知
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	int32 PlayerLevel;
};
