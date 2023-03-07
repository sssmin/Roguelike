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
	void ItemListAnimPlay(UItemInfo* Item) const;
	void RegisterItemEmptySlot(UItemInfo* Item) const;
	void DeactivateOnceItemListWidget() const;
	void SetOwner(APawn* OwnerPawn) const;
	void InitOnceItemList() const;
	
private:
	UPROPERTY(Meta = (BindWidget))
	UOnceItemListWidget* OnceItemListWidget;
	UPROPERTY(Meta = (BindWidget))
	UHPBarWidget* HPBarWidget;

};
