// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIWidget.h"
#include "OnceItemListWidget.h"

void UMainUIWidget::ItemListAnimPlay(FItemInfoTable* Item)
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->ItemListAnimPlay(Item);
	}
}

void UMainUIWidget::RegisterItemEmptySlot(FItemInfoTable* Item)
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->RegisterItem(Item);
	}
}

void UMainUIWidget::DeactiveOnceItemListWidget()
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->DeactiveItemList();
	}
}