// Fill out your copyright notice in the Description page of Project Settings.
#include "StateBoxWidget.h"

#include "PlayerStateWidget.h"
#include "Components/HorizontalBox.h"

void UStateBoxWidget::SetStateIcon(EState State, UTexture2D* Icon)
{
	if (PlayerStateWidgetClass && StateBox)
	{
		UPlayerStateWidget* PlayerStateWidget = CreateWidget<UPlayerStateWidget>(GetWorld(), PlayerStateWidgetClass);
		if (PlayerStateWidget)
		{
			StateBox->AddChildToHorizontalBox(PlayerStateWidget);
			States.Add(State, PlayerStateWidget);
			PlayerStateWidget->SetIcon(Icon);
		}
	}
}

void UStateBoxWidget::SetBuffIcon(EBuff Buff, UTexture2D* Icon)
{
	RemoveBuffIcon(Buff);
	if (PlayerStateWidgetClass && StateBox)
	{
		UPlayerStateWidget* PlayerStateWidget = CreateWidget<UPlayerStateWidget>(GetWorld(), PlayerStateWidgetClass);
		if (PlayerStateWidget)
		{
			StateBox->AddChildToHorizontalBox(PlayerStateWidget);
			Buffs.Add(Buff, PlayerStateWidget);
			PlayerStateWidget->SetIcon(Icon);
		}
	}
}

void UStateBoxWidget::FlickerStateIcon(EState State)
{
	if (States.Contains(State))
	{
		UPlayerStateWidget* PlayerStateWidget = States[State];
		if (PlayerStateWidget)
		{
			PlayerStateWidget->PlayFlicker();
		}
	}
}

void UStateBoxWidget::FlickerBuffIcon(EBuff Buff)
{
	if (Buffs.Contains(Buff))
	{
		UPlayerStateWidget* PlayerStateWidget = Buffs[Buff];
		if (PlayerStateWidget)
		{
			PlayerStateWidget->PlayFlicker();
		}
	}
}

void UStateBoxWidget::RemoveStateIcon(EState State)
{
	if (States.Contains(State))
	{
		UPlayerStateWidget* PlayerStateWidget = States[State];
		if (PlayerStateWidget)
		{
			PlayerStateWidget->RemoveFromParent();
			States.Remove(State);
		}
	}
}

void UStateBoxWidget::RemoveBuffIcon(EBuff Buff)
{
	if (Buffs.Contains(Buff))
	{
		UPlayerStateWidget* PlayerStateWidget = Buffs[Buff];
		if (PlayerStateWidget)
		{
			PlayerStateWidget->RemoveFromParent();
			Buffs.Remove(Buff);
		}
	}
}
