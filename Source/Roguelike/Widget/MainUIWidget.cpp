// Fill out your copyright notice in the Description page of Project Settings.
#include "MainUIWidget.h"

#include "OnceItemListWidget.h"
#include "HPBarWidget.h"

void UMainUIWidget::ItemListAnimPlay(UItemInfo* Item) const
{
	if (OnceItemListWidget && Item) 
	{
		OnceItemListWidget->ItemListAnimPlay(Item);
	}
}

void UMainUIWidget::RegisterItemEmptySlot(UItemInfo* Item) const
{
	if (OnceItemListWidget && Item)
	{
		OnceItemListWidget->RegisterItem(Item);
	}
}

void UMainUIWidget::DeactivateOnceItemListWidget() const
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->DeactivateItemList();
	}
}

void UMainUIWidget::SetOwner(APawn* OwnerPawn) const
{
	if (HPBarWidget && OwnerPawn)
	{
		HPBarWidget->SetOwnerPlayer(OwnerPawn);
	}
}

void UMainUIWidget::InitOnceItemList() const
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->InitItemList();
	}
}

void UMainUIWidget::LoadItem(TArray<UItemInfo*> ItemInfos)
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->LoadItem(ItemInfos);
	}
}
