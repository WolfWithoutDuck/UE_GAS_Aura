// 版权声明--


#include "UI/Widget/EnemyHealthBarWithDebuffWidget.h"

#include "UI/Widget/HpBarDebuffWidget.h"


void UEnemyHealthBarWithDebuffWidget::UpdateDebuffItem( FActiveGameplayEffectHandle& ActiveGameplayEffectHandle)
{
	ClearDebuffItemList();
	if (UHpBarDebuffWidget* DebuffWidget = GetDebuffWidget(ActiveGameplayEffectHandle))
	{
		DebuffWidget->UpdateDebuffWidget(ActiveGameplayEffectHandle);
	}
	else
	{
		// 检查DebuffItem是否有有效的Widget类蓝图指定
		if (DebuffItem)
		{
			// 使用Widget类蓝图创建一个Widget实例
			UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), DebuffItem);
			if (NewWidget && SlotParent)
			{
				// 将新创建的Widget添加到指定的父Widget（SlotParent）中
				NewWidget->AddToViewport();
				SlotParent->AddChild(NewWidget);
				NewWidget->SetVisibility(ESlateVisibility::Visible);
				NewWidget->SetIsEnabled(true);
				// 假设这里需要将新创建的Widget转换为UHpBarDebuffWidget*类型来后续使用，进行类型转换
				if (UHpBarDebuffWidget* NewDebuffWidget = Cast<UHpBarDebuffWidget>(NewWidget))
				{
					// 调用其更新函数进行初始化等操作（根据具体需求调整）
					NewDebuffWidget->UpdateDebuffWidget(ActiveGameplayEffectHandle);
					DebuffItemList.Add(ActiveGameplayEffectHandle, NewDebuffWidget);
				}
			}
		}
	}
}

void UEnemyHealthBarWithDebuffWidget::ClearDebuffItemList()
{
	for (auto It = DebuffItemList.CreateIterator(); It; ++It)
	{
		if (!It.Value()->ItemIsValid())
		{
			It.RemoveCurrent();
		}
	}
}

UHpBarDebuffWidget* UEnemyHealthBarWithDebuffWidget::GetDebuffWidget(FActiveGameplayEffectHandle& ActiveGameplayEffectHandle)
{
	for (auto It = DebuffItemList.CreateIterator(); It; ++It)
	{
		if (It.Key() == ActiveGameplayEffectHandle)
		{
			return It.Value();
		}
	}
	return nullptr;
}
