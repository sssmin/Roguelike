// Fill out your copyright notice in the Description page of Project Settings.
#include "HPBarWidget.h"
#include "Components/ProgressBar.h"

#include "Roguelike/Component/ManagerComponent.h"

void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HPBar)
	{
		HPBar->SetPercent(100.f);
	}
}

void UHPBarWidget::UpdateCurrentHP(float CurrentHP, float MaxHP)
{
	if (HPBar)
	{
		const float Per = CurrentHP / MaxHP;
		HPBar->SetPercent(Per);
	}
}

void UHPBarWidget::SetMonsterType(EMonsterType Type)
{
	if (HPBar)
	{
		switch (Type)
		{
		case EMonsterType::NORMAL:
			HPBar->SetFillColorAndOpacity(FLinearColor::Green);
			break;
		case EMonsterType::ELITE:
			HPBar->SetFillColorAndOpacity(FLinearColor::Red);
			break;
		}
	}
	
}

void UHPBarWidget::SetOwnerPlayer(APawn* OwnerPawn)
{
	UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(OwnerPawn);
	if (ManagerComp)
	{
		ManagerComp->OnUpdateCurrentHP.AddDynamic(this, &ThisClass::UpdateCurrentHP);
	}
}
