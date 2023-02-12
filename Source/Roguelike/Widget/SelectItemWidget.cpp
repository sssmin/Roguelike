// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectItemWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "SelectItemCellWidget.h"
#include "Roguelike/PlayerController/RLPlayerController.h"

void USelectItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HaveEverPressed = false;
	if (ShowAnim)
	{
		PlayAnimation(ShowAnim);
	}
}

void USelectItemWidget::Init(const TArray<FItemInfoTable> Items)
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
	if (!CreatedCellWidgets.IsEmpty())
	{
		for (auto Widget : CreatedCellWidgets)
		{
			Widget->RemoveFromParent();
		}
		CreatedCellWidgets.Empty();
	}

	if (SelectItemCellWidgetClass && LeftItemBox && RightItemBox)
	{
		for (int32 i = 0; i < ItemInfo.Num(); ++i)
		{
			USelectItemCellWidget* CellWidget = CreateWidget<USelectItemCellWidget>(this, SelectItemCellWidgetClass);
			if (CellWidget)
			{
				CellWidget->Init(&ItemInfo[i]);
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
	ARLPlayerController* PC = Cast<ARLPlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->DeactiveOnceItemListWidget();
		if (!HaveEverPressed)
		{
			HaveEverPressed = true;
			Init(PC->GetRandItem());
			CreateCellWidget();
		}
	}
}

void USelectItemWidget::ExitButtonClick()
{
	RemoveFromViewport();
	ARLPlayerController* PC = Cast<ARLPlayerController>(GetOwningPlayer());
	if (PC)
	{
		PC->ResumeController();
		PC->DeactiveOnceItemListWidget();
	}
}
