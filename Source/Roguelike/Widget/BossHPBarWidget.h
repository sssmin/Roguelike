// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPBarWidget.generated.h"

class UProgressBar;
class USizeBox;

UCLASS()
class ROGUELIKE_API UBossHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(APawn* InPawn);
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void UpdateCurrentHP(float CurrentHP, float MaxHP);
	
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* BossHPBar;
	
};
