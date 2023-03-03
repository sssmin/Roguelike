// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Interface/DFSInterface.h"
#include "Roguelike/Type/Types.h"
#include "MinimapWidget.generated.h"

class UUniformGridPanel;
class UMinimapCellWidget;
class UUniformGridSlot;

class UWidgetAnimation;
UCLASS()
class ROGUELIKE_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(int32 Width, int32 Height, TArray<FCell> Board);

private:
	UPROPERTY(Meta = (BindWidget))
	UUniformGridPanel* MapGrid;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMinimapCellWidget> MinimapCellWidgetClass;
	UPROPERTY()
	UMinimapCellWidget* MinimapCellWidget;

	TArray<UMinimapCellWidget*> CellWidgets;
	TArray<UUniformGridSlot*> GridSlots;
	
};
