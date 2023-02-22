// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/ItemManage.h"
#include "ItemComponent.generated.h"

class UManagerComponent;
class ARLPlayerController;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ApplyInfItem(EINFStackItem Item);
	bool ApplyFixMaxItem(EFixMaxStackItem Item);
	bool ApplyOnceEquipItem(const UItemInfo* Item, OUT EOnceEquipItemFlag& Flag);
	bool CheckOnceItem(uint8 Item);
	void ItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem);

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
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
	TMap<uint8, UTexture2D*> ItemIcons;

	void SendManager();
	void ResumeController(ARLPlayerController* RLPC);
	UFUNCTION()
	void SelectItem(UItemInfo* Item);
	UFUNCTION()
	void InitEquipItems();
public:
	FItemManager GetItemManager() const { return ItemManager; }
	void SetManagerComp(UManagerComponent* Comp) { ManagerComp = Comp; }
};
