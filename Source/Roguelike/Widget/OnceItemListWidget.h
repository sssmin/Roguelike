// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/ItemManage.h"
#include "OnceItemListWidget.generated.h"


class UButton;
class UWidgetAnimation;
class UTexture2D;

UCLASS()
class ROGUELIKE_API UOnceItemListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ItemListAnimPlay(UItemInfo* ItemInfo);
	void RegisterItem(UItemInfo* ItemInfo);
	virtual void NativeConstruct() override;
	void DeactivateItemList();
	void InitItemList();

private:
	UFUNCTION()
	void FirstItemButtonClick();
	UFUNCTION()
	void SecondItemButtonClick();
	void SetButtonStyle(UTexture2D* Item, UButton* Btn);
	
	UPROPERTY(Meta = (BindWidget))
	UButton* FirstItemButton;
	UPROPERTY(Meta = (BindWidget))
	UButton* SecondItemButton;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FlikerFirstItem;
	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	UWidgetAnimation* FlikerSecondItem;
	UPROPERTY()
	UItemInfo* FirstItem;
	UPROPERTY()
	UItemInfo* SecondItem;
	UPROPERTY()
	UItemInfo* TempSelectItem;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UTexture2D* OriginSlotImg;
	
	bool bIsActive;
	
};
