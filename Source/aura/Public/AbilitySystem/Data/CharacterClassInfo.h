// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM()
//class 提供了空间作用域的效果，不让此枚举全局读取
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger,
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="默认Class")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};


/**
 * 每个(敌人)角色有自己的主属性，以及通用的根据主属性衍生计算出来的 次要属性，以及重要属性（当前血量等）
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="角色默认Class")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;


	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
