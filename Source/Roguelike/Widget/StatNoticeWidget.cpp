// Fill out your copyright notice in the Description page of Project Settings.


#include "StatNoticeWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UStatNoticeWidget::SetStatNotice(const FString& StatName, const float CurrentValue, const float IncreaseValue, UTexture2D* Icon)
{
	if (StatIcon && StatNameText && CurrentValueText && IncreaseValueText)
	{
		StatIcon->SetBrushFromTexture(Icon);
		StatNameText->SetText(FText::FromString(StatName));
		CurrentValueText->SetText(FText::FromString(FString::FromInt(CurrentValue)));
		IncreaseValueText->SetText(FText::FromString(FString::FromInt(IncreaseValue)));

		StartDelegate.BindDynamic(this, &ThisClass::PlayFadeOut);
		BindToAnimationFinished(FadeIn, StartDelegate);
		EndDelegate.BindDynamic(this, &UUserWidget::RemoveFromParent);
		BindToAnimationFinished(FadeOut, EndDelegate);
		
		PlayFadeIn();
	}
}

void UStatNoticeWidget::PlayFadeIn()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}

void UStatNoticeWidget::PlayFadeOut()
{
	if (FadeOut)
	{
		PlayAnimation(FadeOut);
	}
}
