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
	void ItemListAnimPlay(UItemInfo* ItemInfo);
	void RegisterItem(UItemInfo* ItemInfo);
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

	UItemInfo* FirstItem;
	UItemInfo* SecondItem;
	UItemInfo* TempSelectItem;

	UPROPERTY()
	ARLPlayerController* PC;

	UFUNCTION()
	void FirstItemButtonClick();
	UFUNCTION()
	void SecondItemButtonClick();
	void SetButtonStyle(const UItemInfo* Item, UButton* Btn);

	void RestorePC();
	void RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem);
public:
	bool ExistFirstItem() { return FirstItem != nullptr; }
	bool ExistSecondItem() { return SecondItem != nullptr; }

	
};
