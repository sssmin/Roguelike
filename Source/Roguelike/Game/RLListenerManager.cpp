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

void URLListenerManager::OnNewGame() const
{
	if (OnNewGameDelegate.IsBound())
		OnNewGameDelegate.Broadcast();
}

void URLListenerManager::OnLoadGame() const
{
	if (OnLoadGameDelegate.IsBound())
		OnLoadGameDelegate.Broadcast();
}

void URLListenerManager::OnStartTutorial() const
{
	if (OnStartTutorialDelegate.IsBound())
		OnStartTutorialDelegate.Broadcast();
}

void URLListenerManager::OnStepCompleted() const
{
	OnStepCompletedDelegate.ExecuteIfBound();
}

void URLListenerManager::OnTutorialCompleted() const
{
	OnTutorialCompletedDelegate.ExecuteIfBound();
}
