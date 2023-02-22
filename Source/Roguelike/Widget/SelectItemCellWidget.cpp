// Fill out your copyright notice in the Description page of Project Settings.
#include "SelectItemCellWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"

#include "Roguelike/Game/RLListenerManager.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Kismet/GameplayStatics.h"


void USelectItemCellWidget::Init(UItemInfo* Item)
{
	ItemInfo = Item;

	if (ItemButton && ItemDesc && ItemName && ItemInfo)
	{
		ItemDesc->SetText(FText::FromString(ItemInfo->ItemDesc));
		ItemName->SetText(FText::FromString(ItemInfo->ItemName));
		ItemButton->OnClicked.AddUniqueDynamic(this, &ThisClass::SelectItem);
		FButtonStyle ButtonStyle;
		ButtonStyle.Normal.SetResourceObject(ItemInfo->ItemIcon);
		ButtonStyle.Normal.SetImageSize(FVector2D(256.f, 256.f));
		ButtonStyle.Hovered.SetResourceObject(ItemInfo->ItemIcon);
		ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.5f));
		ButtonStyle.Hovered.SetImageSize(FVector2D(256.f, 256.f));
		ButtonStyle.Pressed.SetResourceObject(ItemInfo->ItemIcon);
		ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.9f));
		ButtonStyle.Pressed.SetImageSize(FVector2D(256.f, 256.f));

		ItemButton->SetStyle(ButtonStyle);
	}
}

void USelectItemCellWidget::SelectItem()
{
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && GI->GetListenerManager())
	{
		GI->GetListenerManager()->SelectItem(ItemInfo);
	}
}