// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLListenerManager.generated.h"

class UItemInfo;

DECLARE_DELEGATE_OneParam(FOnSelectItem, UItemInfo*);
DECLARE_DELEGATE(FRestorePC);
DECLARE_DELEGATE_TwoParams(FRequestItemSwap, const UItemInfo*, const UItemInfo*);
DECLARE_DELEGATE(FDeactiveOnceItemList);
DECLARE_DELEGATE_RetVal(TArray<UItemInfo*>, FGetRandItem);

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
	FDeactiveOnceItemList DeactiveOnceItemListDel;
	void DeactiveOnceItemList();
	FGetRandItem GetRandItemDelegate;
	TArray<UItemInfo*> GetRandItem() const;

};
