// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageWidget.generated.h"


class UTextBlock;
UCLASS()
class ROGUELIKE_API UDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Play(bool IsPlayer, bool IsCritical, float Value, bool IsHeal, bool IsDodge);
	
private:
	void SetColorAndPlay(FLinearColor Color, FText Text);
	
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOut;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ValueText;
	
	
};
