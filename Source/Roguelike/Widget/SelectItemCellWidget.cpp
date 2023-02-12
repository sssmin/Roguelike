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
#include "Roguelike/Component/ItemComponent.h"

void USelectItemCellWidget::Init(FItemInfoTable* Item)
{
	ItemInfo = Item;

	if (ItemButton && ItemDesc && ItemName && ItemInfo)
	{
		ItemDesc->SetText(FText::FromString(ItemInfo->ItemDesc));
		ItemName->SetText(FText::FromString(ItemInfo->ItemName));
		ItemButton->OnClicked.AddDynamic(this, &ThisClass::SelectItem);
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
	ARLPlayerController* PC = Cast<ARLPlayerController>(GetOwningPlayer());
	if (PC && ItemInfo)
	{
		PC->DeactiveOnceItemListWidget();
		APlayerCharacter* Character = Cast<APlayerCharacter>(PC->GetCharacter());
		if (Character && Character->GetItemComp() && Character->GetManagerComp())
		{
			switch (ItemInfo->ItemType)
			{
				case EItemType::INF_STACK_ITEM:
					Character->GetItemComp()->ApplyInfItem(ItemInfo->DetailType.INFStackItem);
					PC->RemoveSelectWidget();
					PC->ResumeController();
					//이펙트, 소리
					break;
				case EItemType::FIX_MAX_STACK_ITEM:
				{
					bool Ret = Character->GetItemComp()->ApplyFixMaxItem(ItemInfo->DetailType.FixMaxStackItem);
					if (!Ret)
					{
						PC->ShowNoticeWidget(TEXT("이미 10번 증가한 스탯이에요."));
						return;
					}
					PC->RemoveSelectWidget();
					PC->ResumeController();
					break;
				}
				case EItemType::ONCE_EQUIP_ITEM:
				{
					EOnceEquipItemFlag Flag;
					bool Ret = Character->GetItemComp()->ApplyOnceEquipItem(ItemInfo, Flag);
					if (Ret)
					{
						switch (Flag)
						{
							case EOnceEquipItemFlag::SUCCESS:
								PC->RemoveSelectWidget();
								PC->ResumeController();
								PC->RegisterItemEmptySlot(ItemInfo);
								break;
							case EOnceEquipItemFlag::EQUIPPED_ALREADY_THIS_ITEM:
							{
								PC->ShowNoticeWidget(TEXT("이미 장착되어 있는 아이템이에요."));
								break;
							}
						}
					}
					else
					{
						PC->ActiveOnceItemListWidget(ItemInfo);
					}
					break;
				}
			}
		}
	}
}