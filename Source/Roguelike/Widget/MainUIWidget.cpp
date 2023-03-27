// Fill out your copyright notice in the Description page of Project Settings.
#include "MainUIWidget.h"

#include "OnceItemListWidget.h"
#include "HPBarWidget.h"
#include "StateBoxWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Roguelike/Component/ManagerComponent.h"

void UMainUIWidget::ItemListAnimPlay(UItemInfo* Item) const
{
	if (OnceItemListWidget && Item) 
	{
		OnceItemListWidget->ItemListAnimPlay(Item);
	}
}

void UMainUIWidget::RegisterItemEmptySlot(UItemInfo* Item) const
{
	if (OnceItemListWidget && Item)
	{
		OnceItemListWidget->RegisterItem(Item);
	}
}

void UMainUIWidget::DeactivateOnceItemListWidget() const
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->DeactivateItemList();
	}
}

void UMainUIWidget::SetOwner(APawn* OwnerPawn)
{
	Init();
	if (HPBarWidget && OwnerPawn)
	{
		HPBarWidget->SetOwnerPlayer(OwnerPawn);
	}
	UManagerComponent* ManagerComp = UManagerComponent::GetManagerComp(OwnerPawn);
	if (ManagerComp)
	{
		ManagerComp->OnSetStateNum.AddDynamic(this, &ThisClass::SetStateNum);
	}
}

void UMainUIWidget::SetStateNum(int32 Value)
{
	if (StateNum)
	{
		StateNum->SetText(FText::FromString(FString::FromInt(Value)));
	}
}

void UMainUIWidget::Init()
{
	if (StateNum)
	{
		StateNum->SetText(FText::FromString("0"));
	}
	if (StateBoxWidget)
	{
		StateBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	IsStateVisible = false;
	
}

void UMainUIWidget::OnClickedStateButton(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (IsStateVisible)
	{
		IsStateVisible = false;
		if (StateBoxWidget)
		{
			StateBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		IsStateVisible = true;
		if (StateBoxWidget)
		{
			StateBoxWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMainUIWidget::InitOnceItemList() const
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->InitItemList();
	}
	if (StateButton)
	{
		//StateButton->IsFocusable = true;
	}
}

void UMainUIWidget::LoadItem(TArray<UItemInfo*> ItemInfos)
{
	if (OnceItemListWidget)
	{
		OnceItemListWidget->LoadItem(ItemInfos);
	}
}

void UMainUIWidget::SetStateIcon(EState State, UTexture2D* Icon) const
{
	if (StateBoxWidget)
	{
		StateBoxWidget->SetStateIcon(State, Icon);
	}
}

void UMainUIWidget::RemoveStateIcon(EState State) const
{
	if (StateBoxWidget)
	{
		StateBoxWidget->RemoveStateIcon(State);
	}
}

void UMainUIWidget::FlickerStateIcon(EState State) const
{
	if (StateBoxWidget)
	{
		StateBoxWidget->FlickerStateIcon(State);
	}
}

void UMainUIWidget::SetBuffIcon(EBuff Buff, UTexture2D* Icon) const
{
	if (StateBoxWidget)
	{
		StateBoxWidget->SetBuffIcon(Buff, Icon);
	}
}

void UMainUIWidget::RemoveBuffIcon(EBuff Buff) const
{
	if (StateBoxWidget)
	{
		StateBoxWidget->RemoveBuffIcon(Buff);
	}
}

void UMainUIWidget::FlickerBuffIcon(EBuff Buff) const
{
	if (StateBoxWidget)
	{
		StateBoxWidget->FlickerBuffIcon(Buff);
	}
}
