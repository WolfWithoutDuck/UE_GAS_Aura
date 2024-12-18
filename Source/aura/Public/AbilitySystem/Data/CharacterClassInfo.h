// 版权声明--

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
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

	UPROPERTY(EditDefaultsOnly, Category="默认Calss")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category= "默认Calss")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category="默认Calss")
	FScalableFloat XPReward = FScalableFloat();
};


/**
 * 每个(敌人)角色有自己的主属性，以及通用的根据主属性衍生计算出来的 次要属性，以及重要属性（当前血量等）
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	//每个角色自己拥有的能力和属性，根据枚举一一映射
	UPROPERTY(EditDefaultsOnly, Category="角色默认Class")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	//共享的派生属性类，主属性，通过存档进行加载设置
	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss")
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;

	//共享的派生属性类
	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	//派生属性，无限时间的GE，区分以上面那个是用于Enemy。敌人生成时候属性就固定了， 不需要根据实际的主属性变化，额外修改派生属性的变化，这个属性用于放在人物身上  
	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss")
	TSubclassOf<UGameplayEffect> SecondaryAttributes_Infinite;

	//共享的重要属性类  最大生命值，魔法等
	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	//共享的通用能力，比如受击打
	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	//共享的伤害计算系数曲线
	UPROPERTY(EditDefaultsOnly, Category= "通用默认Calss|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
