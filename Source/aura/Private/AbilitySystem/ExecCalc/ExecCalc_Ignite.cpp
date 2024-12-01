// 版权声明--


#include "AbilitySystem/ExecCalc/ExecCalc_Ignite.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"


UExecCalc_Ignite::UExecCalc_Ignite()
{
}

void UExecCalc_Ignite::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//获取GE上下文，传递信息
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	const float Damage = UAuraAbilitySystemLibrary::GetIgniteDamage(EffectContextHandle);
	const float Duration = UAuraAbilitySystemLibrary::GetIgniteDuration(EffectContextHandle);


	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TMap<float, float> IgniteDamageStack = AuraEffectContext->GetIgniteCauseDamage();
		AuraEffectContext->CleanUpExpiredEffects(SourceAvatar->GetWorld()->GetTimeSeconds());
		AuraEffectContext->SetIgniteStackInfo(Damage, SourceAvatar->GetWorld()->GetTimeSeconds() + Duration);
	}

	// const FGameplayEffectSpec* GameplayEffect = ExecutionParams.GetOwningSpecForPreExecuteMod();
	// const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	// OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
