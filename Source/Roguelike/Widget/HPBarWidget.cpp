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
	UE_LOG(LogTemp, Warning, TEXT("HPBarWidget SetMonsterType"));
	if (HPBar)
	{
		switch (Type)
		{
		case EMonsterType::Normal:
			HPBar->SetFillColorAndOpacity(FLinearColor::Green);
			break;
		case EMonsterType::Elite:
			HPBar->SetFillColorAndOpacity(FLinearColor::Red);
			break;
		}
	}
}

void UHPBarWidget::SetOwnerPlayer(APawn* Owner)
{
	UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(Owner);
	if (ManagerComp)
	{
		ManagerComp->OnUpdateCurrentHP.AddDynamic(this, &ThisClass::UpdateCurrentHP);
		ManagerComp->HPSync();
	}
}
