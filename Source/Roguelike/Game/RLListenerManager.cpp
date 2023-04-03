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

bool URLListenerManager::HaveCCState() const
{
	return HaveCCStateDelegate.IsBound() ? HaveCCStateDelegate.Execute() : false;
}

void URLListenerManager::OnUpdateCurrentAtk(const float AddToValue) const
{
	OnUpdateCurrentAtkDelegate.ExecuteIfBound(AddToValue);
}

void URLListenerManager::OnUpdateMaxHP(const float AddToValue) const
{
	OnUpdateMaxHPDelegate.ExecuteIfBound(AddToValue);
}

void URLListenerManager::OnHealByValue(const float AddToValue) const
{
	OnHealByValueDelegate.ExecuteIfBound(AddToValue);
}

void URLListenerManager::OnUpdateCurrentRange(const float AddToValue) const
{
	OnUpdateCurrentRangeDele.ExecuteIfBound(AddToValue);
}

void URLListenerManager::OnUpdateCurrentCritical(const float AddToValue) const
{
	OnUpdateCurrentCriticalDele.ExecuteIfBound(AddToValue);
}

bool URLListenerManager::CheckOnceItem(uint8 Item)
{
	return CheckOnceItemDelegate.IsBound() ? CheckOnceItemDelegate.Execute(Item) : false;
}
