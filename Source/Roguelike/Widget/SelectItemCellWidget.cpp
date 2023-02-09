// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectItemCellWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateBrush.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "Roguelike/Component/ManagerComponent.h"

void USelectItemCellWidget::Init(FAllItemTable* Item)
{
	ItemInfo = *Item;

	if (ItemButton && ItemDesc && ItemName)
	{
		ItemDesc->SetText(FText::FromString(ItemInfo.ItemDesc));
		ItemName->SetText(FText::FromString(ItemInfo.ItemName));
		ItemButton->OnClicked.AddDynamic(this, &ThisClass::SelectItem);
		FButtonStyle ButtonStyle;
		ButtonStyle.Normal.SetResourceObject(ItemInfo.ItemIcon);
		ButtonStyle.Normal.SetImageSize(FVector2D(256.f, 256.f));
		ButtonStyle.Hovered.SetResourceObject(ItemInfo.ItemIcon);
		ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.5f));
		ButtonStyle.Hovered.SetImageSize(FVector2D(256.f, 256.f));
		ButtonStyle.Pressed.SetResourceObject(ItemInfo.ItemIcon);
		ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.9f));
		ButtonStyle.Pressed.SetImageSize(FVector2D(256.f, 256.f));

		ItemButton->SetStyle(ButtonStyle);
	}

}

void USelectItemCellWidget::SelectItem()
{
	ARLPlayerController* PC = Cast<ARLPlayerController>(GetOwningPlayer());
	if (PC)
	{
		APlayerCharacter* Character = Cast<APlayerCharacter>(PC->GetCharacter());
		if (Character && Character->GetManagerComp())
		{
			switch (ItemInfo.ItemType)
			{
			case EItemType::INF_STACK_ITEM:
				ItemInfo.EnumNum
				break;
			case EItemType::FIX_MAX_STACK_ITEM:

				break;
			case EItemType::ONCE_EQUIP_ITEM:

				break;
			}
		}
		
		
		PC->RemoveSelectWidget();
		PC->ResumeController();
	}
	
	//이 위젯이 아니라 얘를 가진 위젯이 지워져야함
	//매니저컴포넌트에게 선택하려는 아이템을 넘기고, 결과를 받아야함.
}