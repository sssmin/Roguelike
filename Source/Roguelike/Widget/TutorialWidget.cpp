// Fill out your copyright notice in the Description page of Project Settings.
#include "TutorialWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Roguelike/Game/RLGameInstance.h"

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TypingTime = 0.f;
	bIsActivateTyping = false;
	DisplayText = "";
	bIsTypeComplete = false;
	bIsActivateDescTutorial = true;
	if (SkipButton)
	{
		SkipButton->OnClicked.AddUniqueDynamic(this, &ThisClass::Skip);	
	}
}

void UTutorialWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsActivateTyping)
	{
		TypingTime += InDeltaTime;
		if (TypingTime >= 0.02f)
		{
			DisplayText = UKismetStringLibrary::GetSubstring(FullText, 0, DisplayText.Len() + 1);
			DescText->SetText(FText::FromString(DisplayText));
			TypingTime = 0.f;
		}
		if (DisplayText.Len() == FullText.Len())
		{
			bIsActivateTyping = false;
			bIsTypeComplete = true;
			ClickNoticeText->SetVisibility(ESlateVisibility::Visible);
			PlayAnimation(Flicker, 0, 0.f);
		}
	}
}

FReply UTutorialWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bIsActivateDescTutorial) return FReply::Handled();
	const bool bIsLeftMouseButtonPressed = InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	if (bIsLeftMouseButtonPressed)
	{
		if (!bIsTypeComplete)
		{
			TypingSkip();
		}
		else
		{
			bIsActivateDescTutorial = false;
			const float FadeTime = PlayFadeAnimation(false);
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::TypingComplete, FadeTime, false);
		}
	}
	
	return FReply::Handled();
}

void UTutorialWidget::TypingComplete()
{
	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI && GI->GetListenerManager())
	{
		VisibleDescWidget(false);
		GI->GetListenerManager()->OnStepCompleted();
	}
}

void UTutorialWidget::TypingSkip()
{
	bIsActivateTyping = false;
	TypingTime = 0.f;
	DescText->SetText(FText::FromString(FullText));
	bIsTypeComplete = true;
	ClickNoticeText->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(Flicker, 0, 0.f);
}

void UTutorialWidget::VisibleDescWidget(bool Visible)
{
	if (Visible)
	{
		SetVisibility(ESlateVisibility::Visible);
		FInputModeUIOnly InputMode;
		if (GetOwningPlayer())
		{
			GetOwningPlayer()->SetInputMode(InputMode);
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameAndUI InputMode;
		if (GetOwningPlayer())
		{
			GetOwningPlayer()->SetInputMode(InputMode);
		}
		ClickNoticeText->SetVisibility(ESlateVisibility::Hidden);
		StopAnimation(Flicker);
	}
}

void UTutorialWidget::SetDescText(const FString& Text)
{
	if (GetOwningPlayer())
	{
		DescText->SetText(FText::FromString(TEXT("")));
		bIsActivateDescTutorial = true;
		GetOwningPlayer()->PlayerInput->FlushPressedKeys();
		FullText = Text;
		const float FadeTime = PlayFadeAnimation(true);
		ClickNoticeText->SetVisibility(ESlateVisibility::Hidden);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::ActivateTyping, FadeTime, false);
	}
}

float UTutorialWidget::PlayFadeAnimation(bool IsForward)
{
	if (Fade) //FadeAnim
	{
		if (IsForward)
		{
			PlayAnimation(Fade);
			return Fade->GetEndTime();
		}
		else
		{
			PlayAnimationReverse(Fade);
			return Fade->GetEndTime();
		}
	}
	return 0.f;
}

void UTutorialWidget::Skip()
{
	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI && GI->GetListenerManager())
	{
		GI->GetListenerManager()->OnTutorialCompleted();
	}
	RemoveFromParent();
}

void UTutorialWidget::ActivateTyping()
{
	TypingTime = 0.f;
	DisplayText = "";
	bIsActivateTyping = true; //Activate Tick
	bIsTypeComplete = false;
}
