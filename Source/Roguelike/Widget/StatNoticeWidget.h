// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatNoticeWidget.generated.h"


class UTextBlock;
class UImage;
class UTexture2D;

UCLASS()
class ROGUELIKE_API UStatNoticeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStatNotice(const FString& StatName, const float CurrentValue, const float IncreaseValue, UTexture2D* StatIcon);
	
private:
	void PlayFadeIn();
	UFUNCTION()
	void PlayFadeOut();
	
	
	UPROPERTY(Meta = (BindWidget))
	UImage* StatIcon;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* StatNameText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* CurrentValueText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* IncreaseValueText;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FadeIn;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOut;
	FWidgetAnimationDynamicEvent  StartDelegate;
	FWidgetAnimationDynamicEvent  EndDelegate;
	
};
