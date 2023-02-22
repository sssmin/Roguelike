// Fill out your copyright notice in the Description page of Project Settings.
#include "MainUIWidget.h"

#include "OnceItemListWidget.h"
#include "HPBarWidget.h"

void UMainUIWidget::ItemListAnimPlay(UItemInfo* Item)
{
	if (OnceItemListWidget && Item) 
	{
		OnceItemListWidget->ItemListAnimPlay(Item);
	}
}

void UMainUIWidget::RegisterItemEmptySlot(UItemInfo* Item)
{
	if (OnceItemListWidget && Item)
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

void UMainUIWidget::SetOwner(APawn* OwnerPawn)
{
	if (HPBarWidget && OwnerPawn)
	{
		HPBarWidget->SetOwnerPlayer(OwnerPawn);
	}
}

void UMainUIWidget::InitOnceItemList()
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->InitItemList();
	}
}