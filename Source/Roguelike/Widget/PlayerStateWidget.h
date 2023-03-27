// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateWidget.generated.h"

class UImage;

UCLASS()
class ROGUELIKE_API UPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIcon(UTexture2D* Icon);
	void PlayFlicker();
	void StopFlicker();
	
private:
	UPROPERTY(Meta = (BindWidget))
	UImage* StateIcon;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* Flicker;
	
};
