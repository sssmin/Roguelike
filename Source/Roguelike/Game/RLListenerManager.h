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
DECLARE_MULTICAST_DELEGATE(FOnStartTutorial);
DECLARE_DELEGATE(FOnStepCompleted)
DECLARE_DELEGATE(FOnTutorialCompleted);
DECLARE_DELEGATE_RetVal(bool, FHaveCCState)
DECLARE_DELEGATE_OneParam(FOnUpdateCurrentAtk, const float);
DECLARE_DELEGATE_OneParam(FOnUpdateMaxHP, const float);
DECLARE_DELEGATE_OneParam(FOnHealByValue, const float);
DECLARE_DELEGATE_OneParam(FOnUpdateCurrentRange, const float);
DECLARE_DELEGATE_OneParam(FOnUpdateCurrentCritical, const float);
DECLARE_DELEGATE_RetVal_OneParam(bool, FCheckOnceItem, uint8);

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
	FOnStartTutorial OnStartTutorialDelegate;
	void OnStartTutorial() const;
	FOnStepCompleted OnStepCompletedDelegate;
	void OnStepCompleted() const;
	FOnTutorialCompleted OnTutorialCompletedDelegate;
	UFUNCTION(BlueprintCallable)
	void OnTutorialCompleted() const;
	FHaveCCState HaveCCStateDelegate;
	bool HaveCCState() const;
	FOnUpdateCurrentAtk OnUpdateCurrentAtkDelegate;
	void OnUpdateCurrentAtk(const float AddToValue) const;
	FOnUpdateMaxHP OnUpdateMaxHPDelegate;
	void OnUpdateMaxHP(const float AddToValue) const;
	FOnHealByValue OnHealByValueDelegate;
	void OnHealByValue(const float AddToValue) const;
	FOnUpdateCurrentRange OnUpdateCurrentRangeDele;
	void OnUpdateCurrentRange(const float AddToValue) const;
	FOnUpdateCurrentCritical OnUpdateCurrentCriticalDele;
	void OnUpdateCurrentCritical(const float AddToValue) const;
	FCheckOnceItem CheckOnceItemDelegate;
	bool CheckOnceItem(uint8 Item);
	
};
