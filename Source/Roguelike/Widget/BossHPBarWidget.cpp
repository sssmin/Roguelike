// Fill out your copyright notice in the Description page of Project Settings.
#include "BossHPBarWidget.h"
#include "Components/ProgressBar.h"

#include "Roguelike/Component/ManagerComponent.h"

void UBossHPBarWidget::Init(APawn* InPawn)
{
	UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(InPawn);
	if (ManagerComp)
	{
		ManagerComp->OnUpdateCurrentHP.AddDynamic(this, &ThisClass::UpdateCurrentHP);
	}
}

void UBossHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BossHPBar)
	{
		BossHPBar->SetPercent(100.f);
	}
}

void UBossHPBarWidget::UpdateCurrentHP(float CurrentHP, float MaxHP)
{
	if (BossHPBar)
	{
		const float Per = CurrentHP / MaxHP;
		BossHPBar->SetPercent(Per);
	}
}
