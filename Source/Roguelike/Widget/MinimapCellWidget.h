// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/Types.h"
#include "MinimapCellWidget.generated.h"

class UBorder;
class UWidgetAnimation;

UCLASS()
class ROGUELIKE_API UMinimapCellWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(ECellState RoomState);
private:
	UPROPERTY(Meta = (BindWidget))
	UBorder* Cell;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FlikerAnim;
	
};
