// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoticeWidget.generated.h"


class UTextBlock;

UCLASS()
class ROGUELIKE_API UNoticeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetText(const FString& Text);

private:
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Notice;
	
};
