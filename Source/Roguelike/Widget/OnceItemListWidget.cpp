// Fill out your copyright notice in the Description page of Project Settings.


#include "OnceItemListWidget.h"
#include "Components/Button.h"
#include "Roguelike/PlayerController/RLPlayerController.h"

void UOnceItemListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (FirstItemButton && SecondItemButton)
	{
		FirstItemButton->OnClicked.AddUniqueDynamic(this, &ThisClass::FirstItemButtonClick);
		SecondItemButton->OnClicked.AddUniqueDynamic(this, &ThisClass::SecondItemButtonClick);
	}
	bIsEnabled = false;

	PC = Cast<ARLPlayerController>(GetOwningPlayer());
	
}

void UOnceItemListWidget::ItemListAnimPlay(UItemInfo* ItemInfo)
{
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
			SetButtonStyle(FirstItem, FirstItemButton);
		}
	}
	else
	{
		SecondItem = ItemInfo;
		if (SecondItemButton && SecondItem)
		{
			SetButtonStyle(SecondItem, SecondItemButton);
		}
	}
}

void UOnceItemListWidget::SetButtonStyle(const UItemInfo* Item, UButton* Btn)
{	
	FButtonStyle ButtonStyle;
	ButtonStyle.Normal.SetResourceObject(Item->ItemIcon);
	ButtonStyle.Normal.SetImageSize(FVector2D(256.f, 256.f));
	ButtonStyle.Hovered.SetResourceObject(Item->ItemIcon);
	ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.5f));
	ButtonStyle.Hovered.SetImageSize(FVector2D(256.f, 256.f));
	ButtonStyle.Pressed.SetResourceObject(Item->ItemIcon);
	ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.9f));
	ButtonStyle.Pressed.SetImageSize(FVector2D(256.f, 256.f));
	Btn->SetStyle(ButtonStyle);
}

void UOnceItemListWidget::FirstItemButtonClick()
{
	if (FirstItemButton && FirstItem && TempSelectItem)
	{
		RequestItemSwap(FirstItem, TempSelectItem);
		FirstItem = TempSelectItem;
		SetButtonStyle(FirstItem, FirstItemButton);
		DeactiveItemList();                        
		RestorePC();
	}
}

void UOnceItemListWidget::SecondItemButtonClick()
{
	if (SecondItemButton && SecondItem && TempSelectItem)
	{
		RequestItemSwap(SecondItem, TempSelectItem);
		SecondItem = TempSelectItem;
		SetButtonStyle(SecondItem, SecondItemButton);
		DeactiveItemList();
		RestorePC();
	}
}

void UOnceItemListWidget::DeactiveItemList()
{
	FirstItemButton->SetIsEnabled(false);
	SecondItemButton->SetIsEnabled(false);

	StopAnimation(FlikerFirstItem);
	StopAnimation(FlikerSecondItem);

	TempSelectItem = nullptr;
}

void UOnceItemListWidget::RestorePC()
{
	if (PC)
	{
		PC->RemoveSelectWidget();
		PC->ResumeController();
	}
}

void UOnceItemListWidget::RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem)
{
	if (PC)
	{
		PC->RequestItemSwap(OldItem, NewItem);
	}
}