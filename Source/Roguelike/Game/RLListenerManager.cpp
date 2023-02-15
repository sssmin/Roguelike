// Fill out your copyright notice in the Description page of Project Settings.


#include "RLListenerManager.h"

URLListenerManager* URLListenerManager::ListenerInstance = nullptr;

URLListenerManager::URLListenerManager()
{
}

URLListenerManager::~URLListenerManager()
{
}

URLListenerManager* URLListenerManager::Get()
{
	return ListenerInstance ? ListenerInstance : ListenerInstance = NewObject<URLListenerManager>();
}

void URLListenerManager::SelectItem(UItemInfo* ItemInfo) const
{
	if (OnSelectItem.IsBound())
	{
		OnSelectItem.Broadcast(ItemInfo);
	}
}

