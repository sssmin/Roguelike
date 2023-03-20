// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/ItemManage.h"
#include "SelectItemCellWidget.generated.h"


class UButton;
class UTextBlock;

UCLASS()
class ROGUELIKE_API USelectItemCellWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(UItemInfo* Item, bool IsTutorial);
	
private:
	UFUNCTION()
	void SelectItem();
	
	UPROPERTY(Meta = (BindWidget))
	UButton* ItemButton;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ItemDesc;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ItemName;
	UPROPERTY()
	UItemInfo* ItemInfo;

	bool bIsTutorial;

public:
	UItemInfo* GetItemInfo() const { return ItemInfo; }
	
};
