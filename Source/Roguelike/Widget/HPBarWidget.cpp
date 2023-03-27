// Fill out your copyright notice in the Description page of Project Settings.
#include "HPBarWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

#include "Roguelike/Component/ManagerComponent.h"

void UHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HPBar)
	{
		HPBar->SetPercent(100.f);
	}
	if (StateIcon)
	{
		StateIcon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHPBarWidget::SetImage(UTexture2D* Icon)
{
	if (StateIcon && Icon)
	{
		StateIcon->SetVisibility(ESlateVisibility::Visible);
		StateIcon->SetBrushFromTexture(Icon, true);
	}
	if (IsAnimationPlaying(Flicker))
	{
		StopAnimation(Flicker);
	}
}

void UHPBarWidget::RemoveImage()
{
	if (StateIcon)
	{
		StateIcon->SetVisibility(ESlateVisibility::Hidden);
	}
	if (IsAnimationPlaying(Flicker))
	{
		StopAnimation(Flicker);
	}
}

void UHPBarWidget::StartFlicker()
{
	if (Flicker)
	{
		PlayAnimation(Flicker, 0, 0);
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
		case EMonsterType::Normal:
			HPBar->SetFillColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
			break;
		case EMonsterType::Elite:
			HPBar->SetFillColorAndOpacity(FLinearColor(0.1f, 0.f, 0.f, 1.f));
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
