// Fill out your copyright notice in the Description page of Project Settings.
#include "OnceItemListWidget.h"
#include "Components/Button.h"

#include "Roguelike/Game/RLListenerManager.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UOnceItemListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (FirstItemButton && SecondItemButton)
	{
		FirstItemButton->OnClicked.AddUniqueDynamic(this, &ThisClass::FirstItemButtonClick);
		SecondItemButton->OnClicked.AddUniqueDynamic(this, &ThisClass::SecondItemButtonClick);
	}
	
	bIsActive = false;
}

void UOnceItemListWidget::ItemListAnimPlay(UItemInfo* ItemInfo)
{
	if (bIsActive) return;
	bIsActive = true;
	TempSelectItem = ItemInfo;

	if (FlikerFirstItem && FlikerSecondItem && FirstItemButton && SecondItemButton)
	{
		FirstItemButton->SetIsEnabled(true);
		SecondItemButton->SetIsEnabled(true);
		
		PlayAnimation(FlikerFirstItem, 0.f, 0);
		PlayAnimation(FlikerSecondItem, 0.f, 0);
	}
}

void UOnceItemListWidget::RegisterItem(UItemInfo* ItemInfo)
{
	if (!FirstItem)
	{
		FirstItem = ItemInfo;
	
		if (FirstItemButton && FirstItem)
		{
			SetButtonStyle(FirstItem->ItemIcon, FirstItemButton);
		}
	}
	else
	{
		SecondItem = ItemInfo;
		if (SecondItemButton && SecondItem)
		{
			SetButtonStyle(SecondItem->ItemIcon, SecondItemButton);
		}
	}
}

void UOnceItemListWidget::SetButtonStyle(UTexture2D* ItemIcon, UButton* Btn)
{	
	FButtonStyle ButtonStyle;
	ButtonStyle.Normal.SetResourceObject(ItemIcon);
	ButtonStyle.Normal.SetImageSize(FVector2D(256.f, 256.f));
	ButtonStyle.Hovered.SetResourceObject(ItemIcon);
	ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.5f));
	ButtonStyle.Hovered.SetImageSize(FVector2D(256.f, 256.f));
	ButtonStyle.Pressed.SetResourceObject(ItemIcon);
	ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.9f));
	ButtonStyle.Pressed.SetImageSize(FVector2D(256.f, 256.f));
	Btn->SetStyle(ButtonStyle);
}

void UOnceItemListWidget::FirstItemButtonClick()
{
	if (FirstItemButton && FirstItem && TempSelectItem)
	{
		URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
		if (GI && GI->GetListenerManager())
		{
			GI->GetListenerManager()->RequestItemSwap(FirstItem, TempSelectItem);
			FirstItem = TempSelectItem;
			SetButtonStyle(FirstItem->ItemIcon, FirstItemButton);
			DeactivateItemList();
			GI->GetListenerManager()->RestorePC();
		}
	}
}

void UOnceItemListWidget::SecondItemButtonClick()
{
	if (SecondItemButton && SecondItem && TempSelectItem)
	{
		URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
		if (GI && GI->GetListenerManager())
		{
			GI->GetListenerManager()->RequestItemSwap(SecondItem, TempSelectItem);
			SecondItem = TempSelectItem;
			SetButtonStyle(SecondItem->ItemIcon, SecondItemButton);
			DeactivateItemList();
			GI->GetListenerManager()->RestorePC();
		}
	}
}

void UOnceItemListWidget::DeactivateItemList()
{
	if (!bIsActive) return;
	bIsActive = false;
	FirstItemButton->SetIsEnabled(false);
	SecondItemButton->SetIsEnabled(false);

	StopAnimation(FlikerFirstItem);
	StopAnimation(FlikerSecondItem);

	TempSelectItem = nullptr;
}

void UOnceItemListWidget::InitItemList()
{
	FirstItem = nullptr;
	SecondItem = nullptr;
	TempSelectItem = nullptr;
	
	SetButtonStyle(OriginSlotImg, FirstItemButton);
	SetButtonStyle(OriginSlotImg, SecondItemButton);
}

void UOnceItemListWidget::LoadItem(TArray<UItemInfo*> ItemInfos)
{
	for (int32 i = 0; i < ItemInfos.Num(); ++i)
	{
		if (i == 0)
		{
			FirstItem = ItemInfos[i];
			if (FirstItem && FirstItemButton)
				SetButtonStyle(FirstItem->ItemIcon, FirstItemButton);
		}
		else
		{
			SecondItem = ItemInfos[i];
			if (SecondItem && SecondItemButton)
			SetButtonStyle(SecondItem->ItemIcon, SecondItemButton);
		}
	}
}
