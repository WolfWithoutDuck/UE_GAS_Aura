// 版权声明--


#include "AbilitySystem/MMC/MMC_IgniteDamage.h"

#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAttributeSet.h"


UMMC_IgniteDamage::UMMC_IgniteDamage()
{
	EffectiveIgniteStackDef.AttributeToCapture = UAuraAttributeSet::GetEffectiveIgniteStackAttribute();
	EffectiveIgniteStackDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	EffectiveIgniteStackDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(EffectiveIgniteStackDef);
}

float UMMC_IgniteDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTag;


	float EffectiveIgniteStack = 1.f;
	GetCapturedAttributeMagnitude(EffectiveIgniteStackDef, Spec, EvaluateParameters, EffectiveIgniteStack);
	EffectiveIgniteStack = FMath::Max<float>(EffectiveIgniteStack, 1.f);

	//获取伤害，大伤覆盖小伤，且根据生效堆栈，获取栈顶的数值总和
	float Damage = 0;
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Spec.GetContext().Get()))
	{
		Damage = AuraEffectContext->GetTotalIgniteDamage(AuraEffectContext->GetSourceObject()->GetWorld()->GetTimeSeconds(), EffectiveIgniteStack);
	}
	return Damage;
}
