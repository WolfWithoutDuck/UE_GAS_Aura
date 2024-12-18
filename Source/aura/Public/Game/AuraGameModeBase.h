// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/LootTier.h"
#include "GameFramework/GameModeBase.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "AuraGameModeBase.generated.h"

class USaveGame;
class UAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="默认角色Calss")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;


	UPROPERTY(EditDefaultsOnly, Category="Loot Tiers")
	TObjectPtr<ULootTier> LootTiers;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex);

	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	ULoadScreenSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);
	void SaveWorldState(UWorld* World, const FString& InDestinationWorldAssetName = FString(""));
	void LoadWorldState(UWorld* World) const;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	void TravelToMap(UMVVM_LoadSlot* Slot);

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerDied(const ACharacter* DeadCharacter);

protected:
	virtual void BeginPlay() override;
};
