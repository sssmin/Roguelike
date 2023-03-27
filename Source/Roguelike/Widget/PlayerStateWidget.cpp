// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerStateWidget.h"

#include "Components/Image.h"

void UPlayerStateWidget::SetIcon(UTexture2D* Icon)
{
	if (StateIcon && Icon)
	{
		StateIcon->SetVisibility(ESlateVisibility::Visible);
		StateIcon->SetBrushFromTexture(Icon, true);
	}
}

void UPlayerStateWidget::PlayFlicker()
{
	if (Flicker)
	{
		PlayAnimation(Flicker, 0.f, 0);
	}
}

void UPlayerStateWidget::StopFlicker()
{
	if (IsAnimationPlaying(Flicker))
	{
		StopAnimation(Flicker);
	}
}
