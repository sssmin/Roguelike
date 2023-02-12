// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainUIWidget.generated.h"

class UOnceItemListWidget;
struct FItemInfoTable;

UCLASS()
class ROGUELIKE_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void ItemListAnimPlay(FItemInfoTable* Item);
	void RegisterItemEmptySlot(FItemInfoTable* Item);
	void DeactiveOnceItemListWidget();
private:
	UPROPERTY(Meta = (BindWidget))
	UOnceItemListWidget* OnceItemListWidget;


};
