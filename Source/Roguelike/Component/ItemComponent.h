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
	UFUNCTION(BlueprintCallable)
	void Init();
	void ApplyInfItem(EINFStackItem Item);
	bool ApplyFixMaxItem(EFixMaxStackItem Item);
	bool ApplyOnceEquipItem(const UItemInfo* Item, OUT EOnceEquipItemFlag& Flag);
	bool CheckOnceItem(uint8 Item);
	void ItemSwap(UItemInfo* OldItem, UItemInfo* NewItem);

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	
private:
	UFUNCTION()
	void SelectItem(UItemInfo* Item);
	UFUNCTION()
	void InitEquipItems();
	UFUNCTION()
	void SetTempManager() const;
	void ApplyOnceItem(uint8 Item);
	void RemoveOnceItem(uint8 Item);
	uint8 GetFixMaxStack(EFixMaxStackItem Item);
	void IncreaseFixMaxStack(EFixMaxStackItem Item);
	void ResumeController(ARLPlayerController* RLPC);
	float CalcStatValuePer(const TArray<float>& Per, const TArray<int32>& Value);
	
	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	FItemManager ItemManager;
	UPROPERTY()
	UManagerComponent* ManagerComp;
	
	const uint8 FIX_MAX_STACK = 10;
	
	UPROPERTY()
	TMap<uint8, uint8> FixMaxNum;
	UPROPERTY()
	TArray<UItemInfo*> ItemInfos;
	
public:
	FItemManager GetItemManager() const { return ItemManager; }
	void SetManagerComp(UManagerComponent* Comp) { ManagerComp = Comp; }
	TArray<UItemInfo*> GetItemInfos() const { return ItemInfos; }
};
