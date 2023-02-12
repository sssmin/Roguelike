// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/ItemManage.h"
#include "ItemComponent.generated.h"

class UManagerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ApplyInfItem(EINFStackItem Item);
	bool ApplyFixMaxItem(EFixMaxStackItem Item);
	bool ApplyOnceEquipItem(FItemInfoTable* Item, OUT EOnceEquipItemFlag& Flag);
	bool CheckOnceItem(uint8 Item);
	void ItemSwap(const FItemInfoTable* OldItem, const FItemInfoTable* NewItem);
protected:
	virtual void BeginPlay() override;

private:	
	const char FIX_MAX_STACK = 10;

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	FItemManager ItemManager;
	void ApplyOnceItem(uint8 Item);
	void RemoveOnceItem(uint8 Item);
	uint8 GetFixMaxStack(EFixMaxStackItem Item);
	void IncreaseFixMaxStack(EFixMaxStackItem Item);

	UPROPERTY()
	UManagerComponent* ManagerComp;
	
	float IncreaseAtkValue;
	float IncreaseMaxHpValue;
	TMap<uint8, uint8> FixMaxNum;
	
public:
	FItemManager GetItemManager() const { return ItemManager; }
	void SetManagerComp(UManagerComponent* Comp) { ManagerComp = Comp; }
};
