// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLListenerManager.generated.h"

class UItemInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectItem, UItemInfo*, ItemInfo);

UCLASS()
class ROGUELIKE_API URLListenerManager : public UObject
{
	GENERATED_BODY()
public:
	URLListenerManager();
	~URLListenerManager();
	static URLListenerManager* Get();
	static URLListenerManager* ListenerInstance;

	FOnSelectItem OnSelectItem;
	void SelectItem(UItemInfo* ItemInfo) const;
};
