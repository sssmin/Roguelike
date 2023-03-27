// Fill out your copyright notice in the Description page of Project Settings.
#include "DamageWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UDamageWidget::Play(bool IsPlayer, bool IsCritical, float Value, bool IsHeal, bool IsDodge)
{
	if (FadeOut && ValueText)
	{
		TEnumAsByte<ERoundingMode> Mode = ERoundingMode::HalfToEven;

		FNumberFormattingOptions Options;
		Options.RoundingMode = ERoundingMode::HalfToEven;
		Options.SetMinimumFractionalDigits(0);
		Options.SetMaximumFractionalDigits(1);
		FText TextFromInt = FText::AsNumber(Value, &Options);
		FLinearColor Color = FLinearColor();
		
		if (IsDodge)
		{
			Color = FLinearColor(1.f, 1.f, 1.f, 1.f);
			TextFromInt = FText::FromString(TEXT("DODGE!"));
		}
		else if (IsHeal)
		{
			Color = FLinearColor(0.f, 0.770833f, 0.f, 1.f);
		}
		else if (IsPlayer)
		{
			Color = FLinearColor(0.770833f, 0.f, 0.f, 1.f);
		}
		else if (!IsPlayer)
		{
			Color = FLinearColor(1.f, 1.f, 0.f, 1.f);
			if (IsCritical)
			{
				Color = FLinearColor(1.f, 0.3f, 0.f, 1.f);
			}
		}
		SetColorAndPlay(Color, TextFromInt);
	}
}

void UDamageWidget::SetColorAndPlay(FLinearColor Color, FText Text)
{
	ValueText->SetColorAndOpacity(Color);
	ValueText->SetText(Text);
	PlayAnimation(FadeOut);
}