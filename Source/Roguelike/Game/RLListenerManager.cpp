// Fill out your copyright notice in the Description page of Project Settings.
#include "RLListenerManager.h"


void URLListenerManager::SelectItem(UItemInfo* ItemInfo) const
{
	OnSelectItemDelegate.ExecuteIfBound(ItemInfo);
}

void URLListenerManager::RestorePC()
{
	RestorePCDelegate.ExecuteIfBound();
}

void URLListenerManager::RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem)
{
	RequestItemSwapDelegate.ExecuteIfBound(OldItem, NewItem);
}

void URLListenerManager::DeactivateOnceItemList()
{
	DeactivateOnceItemListDel.ExecuteIfBound();
}

TArray<UItemInfo*> URLListenerManager::GetRandItem() const
{
	return GetRandItemDelegate.Execute();
}
