// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/Item.h"
#include "SelectItemCellWidget.generated.h"


class UButton;
class UTextBlock;

UCLASS()
class ROGUELIKE_API USelectItemCellWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(FAllItemTable* Item);
private:
	UPROPERTY(Meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ItemDesc;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ItemName;

	FAllItemTable ItemInfo;

	UFUNCTION()
	void SelectItem();
};
