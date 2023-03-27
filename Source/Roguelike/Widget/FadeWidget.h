// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeWidget.generated.h"

UCLASS()
class ROGUELIKE_API UFadeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	float PlayFadeOut();

private:
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOut;
};
