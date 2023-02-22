// Fill out your copyright notice in the Description page of Project Settings.
#include "HPBarWidget.h"
#include "Components/ProgressBar.h"

#include "Roguelike/Character/BaseCharacter.h"
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

void UHPBarWidget::SetOwnerPlayer(APawn* OwnerPawn)
{
	OwnerCharacter = Cast<ABaseCharacter>(OwnerPawn);

	if (OwnerCharacter)
	{
		UManagerComponent* ManagerComp = OwnerCharacter->GetManagerComp();
		if (ManagerComp)
		{
			ManagerComp->OnUpdateCurrentHP.AddDynamic(this, &ThisClass::UpdateCurrentHP);
		}
	}
}