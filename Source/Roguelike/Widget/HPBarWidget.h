// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/DTForGM.h"
#include "HPBarWidget.generated.h"

class UImage;
class UProgressBar;

UCLASS()
class ROGUELIKE_API UHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwnerPlayer(APawn* OwnerPawn);
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void UpdateCurrentHP(float CurrentHP, float MaxHP);
	
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* HPBar;
	UPROPERTY(Meta = (BindWidget))
	UImage* Status;
	
public:
	void SetMonsterType(EMonsterType Type);
	
};
