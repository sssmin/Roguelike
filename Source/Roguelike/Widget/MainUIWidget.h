// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Type/StatManage.h"
#include "MainUIWidget.generated.h"

class UTextBlock;
class UStateBoxWidget;
class UImage;
class UHorizontalBox;
class UOnceItemListWidget;
class UHPBarWidget;
class UItemInfo;

UCLASS()
class ROGUELIKE_API UMainUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ItemListAnimPlay(UItemInfo* Item) const;
	void RegisterItemEmptySlot(UItemInfo* Item) const;
	void DeactivateOnceItemListWidget() const;
	void SetOwner(APawn* OwnerPawn);
	void InitOnceItemList() const;
	void LoadItem(TArray<UItemInfo*> ItemInfos);
	void SetStateIcon(EState State, UTexture2D* Icon) const;
	void RemoveStateIcon(EState State) const;
	void FlickerStateIcon(EState State) const;
	void SetBuffIcon(EBuff Buff, UTexture2D* Icon) const;
	void RemoveBuffIcon(EBuff Buff) const;
	void FlickerBuffIcon(EBuff Buff) const;
	
private:
	void Init();
	UFUNCTION()
	void SetStateNum(int32 Value);
	UFUNCTION()
	void OnClickedStateButton(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget, AllowPrivateAccess = "true"))
	UOnceItemListWidget* OnceItemListWidget;
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget, AllowPrivateAccess = "true"))
	UHPBarWidget* HPBarWidget;
	UPROPERTY(Meta = (BindWidget))
	UImage* StateButton;
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* StateNum;
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget, AllowPrivateAccess = "true"))
	UStateBoxWidget* StateBoxWidget;
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool IsStateVisible;
	
};
