// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"


class UButton;
class UTextBlock;
UCLASS()
class ROGUELIKE_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDescText(const FString& Text);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void TypingSkip();
	void VisibleDescWidget(bool Visible);
	
private:
	void ActivateTyping();
	void TypingComplete();
	float PlayFadeAnimation(bool IsForward);
	UFUNCTION()
	void Skip();
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* DescText;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ClickNoticeText;
	UPROPERTY(Meta = (BindWidget))
	UButton* SkipButton;

	FString DisplayText;
	FString FullText;
	float TypingTime;
	bool bIsActivateTyping;
	bool bIsTypeComplete;

	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* Fade;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* Flicker;
	bool bIsActivateDescTutorial;
public:
	bool GetIsTypeCompleted() const { return bIsTypeComplete; }
	
};
