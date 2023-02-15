// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBarWidget.generated.h"

class UProgressBar;
class ABaseCharacter;

UCLASS()
class ROGUELIKE_API UHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwnerPlayer(APawn* OwnerPawn);
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* HPBar;
	UFUNCTION()
	void UpdateCurrentHP(float CurrentHP, float MaxHP);

	UPROPERTY()
	ABaseCharacter* OwnerCharacter;
};
