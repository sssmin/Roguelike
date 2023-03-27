// Fill out your copyright notice in the Description page of Project Settings.
#include "FadeWidget.h"

#include "Animation/WidgetAnimation.h"

float UFadeWidget::PlayFadeOut()
{
	if (FadeOut)
	{
		PlayAnimation(FadeOut);
		return FadeOut->GetEndTime();
	}
	return 0.f;
}
