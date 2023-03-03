// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainUIWidget.generated.h"

class UOnceItemListWidget;
class UHPBarWidget;
class UItemInfo;

UCLASS()
class ROGUELIKE_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ItemListAnimPlay(UItemInfo* Item);
	void RegisterItemEmptySlot(UItemInfo* Item);
	void DeactiveOnceItemListWidget();
	void SetOwner(APawn* OwnerPawn);
	void InitOnceItemList();
	
private:
	UPROPERTY(Meta = (BindWidget))
	UOnceItemListWidget* OnceItemListWidget;
	UPROPERTY(Meta = (BindWidget))
	UHPBarWidget* HPBarWidget;

};
