// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLListenerManager.generated.h"

class UItemInfo;

DECLARE_DELEGATE_OneParam(FOnSelectItem, UItemInfo*);
DECLARE_DELEGATE(FRestorePC);
DECLARE_DELEGATE_TwoParams(FRequestItemSwap, const UItemInfo*, const UItemInfo*);
DECLARE_DELEGATE(FDeactivateOnceItemList);
DECLARE_DELEGATE_RetVal(TArray<UItemInfo*>, FGetRandItem);
DECLARE_MULTICAST_DELEGATE(FOnNewGame);
DECLARE_MULTICAST_DELEGATE(FOnLoadGame);

UCLASS()
class ROGUELIKE_API URLListenerManager : public UObject
{
	GENERATED_BODY()
	
public:
	FOnSelectItem OnSelectItemDelegate;
	void SelectItem(UItemInfo* ItemInfo) const;
	FRestorePC RestorePCDelegate;
	void RestorePC();
	FRequestItemSwap RequestItemSwapDelegate;
	void RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem);
	FDeactivateOnceItemList DeactivateOnceItemListDel;
	void DeactivateOnceItemList();
	FGetRandItem GetRandItemDelegate;
	TArray<UItemInfo*> GetRandItem() const;
	FOnNewGame OnNewGameDelegate;
	void OnNewGame() const;
	FOnLoadGame OnLoadGameDelegate;
	void OnLoadGame() const;

};
