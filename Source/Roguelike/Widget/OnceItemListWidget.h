// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/ItemManage.h"
#include "OnceItemListWidget.generated.h"


class UButton;
class UWidgetAnimation;
class ARLPlayerController;

UCLASS()
class ROGUELIKE_API UOnceItemListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ItemListAnimPlay(FItemInfoTable* ItemInfo);
	void RegisterItem(FItemInfoTable* ItemInfo);
	virtual void NativeConstruct() override;
	void DeactiveItemList();
private:
	UPROPERTY(Meta = (BindWidget))
	UButton* FirstItemButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* SecondItemButton;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FlikerFirstItem;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FlikerSecondItem;

	FItemInfoTable* FirstItem;
	FItemInfoTable* SecondItem;
	FItemInfoTable* TempSelectItem;

	UPROPERTY()
	ARLPlayerController* PC;

	UFUNCTION()
	void FirstItemButtonClick();
	UFUNCTION()
	void SecondItemButtonClick();
	void SetButtonStyle(FItemInfoTable* Item, UButton* Btn);

	void RestorePC();
	void RequestItemSwap(const FItemInfoTable* OldItem, const FItemInfoTable* NewItem);
public:
	bool ExistFirstItem() { return FirstItem != nullptr; }
	bool ExistSecondItem() { return SecondItem != nullptr; }

	
};
