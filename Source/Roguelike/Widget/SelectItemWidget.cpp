// Fill out your copyright notice in the Description page of Project Settings.
#include "SelectItemWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"

#include "SelectItemCellWidget.h"
#include "Roguelike/Game/RLListenerManager.h"
#include "Roguelike/Game/RLGameInstance.h"

void USelectItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HaveEverPressed = false;
	if (ShowAnim)
	{
		PlayAnimation(ShowAnim);
	}
}

void USelectItemWidget::Init(const TArray<UItemInfo*> Items)
{
	ItemInfo = Items;
	if (F5Button && ExitButton)
	{
		F5Button->OnClicked.AddUniqueDynamic(this, &ThisClass::F5ButtonClick);
		ExitButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ExitButtonClick);
	}
}

void USelectItemWidget::CreateCellWidget()
{
	if (SelectItemCellWidgetClass && LeftItemBox && RightItemBox)
	{
		for (int32 i = 0; i < ItemInfo.Num(); ++i)
		{
			USelectItemCellWidget* CellWidget = CreateWidget<USelectItemCellWidget>(this, SelectItemCellWidgetClass);
			if (CellWidget)
			{
				CellWidget->Init(ItemInfo[i]);
				
				CreatedCellWidgets.Add(CellWidget);
			}

			if (i == 0)
			{
				if (CreatedCellWidgets.IsValidIndex(i))
				{
					UVerticalBoxSlot* BoxSlot = LeftItemBox->AddChildToVerticalBox(CreatedCellWidgets[i]);
					if (BoxSlot)
					{
						BoxSlot->Size = FSlateChildSize();
					}
				}
			}
			else
			{
				if (CreatedCellWidgets.IsValidIndex(i))
				{
					UVerticalBoxSlot* BoxSlot = RightItemBox->AddChildToVerticalBox(CreatedCellWidgets[i]);
					if (BoxSlot)
					{
						BoxSlot->Size = FSlateChildSize();
					}
				}
			}
		}
	}
}

void USelectItemWidget::F5ButtonClick()
{
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && GI->GetListenerManager())
	{
		GI->GetListenerManager()->DeactivateOnceItemList();
		if (!HaveEverPressed)
		{
			HaveEverPressed = true;
			TArray<UItemInfo*> ItemInfos = GI->GetListenerManager()->GetRandItem();
			Init(ItemInfos);
			Refresh();
		}
	}
}

void USelectItemWidget::ExitButtonClick()
{
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI && GI->GetListenerManager())
	{
		GI->GetListenerManager()->RestorePC();
	}
}

void USelectItemWidget::Refresh()
{
	if (!CreatedCellWidgets.IsEmpty())
	{
		for (int32 i = 0; i < ItemInfo.Num(); ++i)
		{
			CreatedCellWidgets[i]->Init(ItemInfo[i]);
		}
	}
}