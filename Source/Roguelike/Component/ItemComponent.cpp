// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemComponent.h"
#include "ManagerComponent.h"
#include "Roguelike/PlayerController/RLPlayerController.h"

UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ItemManager = FItemManager();
	IncreaseAtkValue = 3.f;
	IncreaseMaxHpValue = 15.f;

}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UItemComponent::CheckOnceItem(uint8 Item)
{
	return ItemManager.OnceEquippedItem & Item;
}

void UItemComponent::ApplyOnceItem(uint8 Item)
{
	ItemManager.OnceEquippedItem |= Item;
}

void UItemComponent::RemoveOnceItem(uint8 Item)
{
	ItemManager.OnceEquippedItem ^= Item;
}

void UItemComponent::ApplyInfItem(EINFStackItem Item)
{
	if (ManagerComp)
	{
		switch (Item)
		{
		case EINFStackItem::INCREASE_ATK:
			ManagerComp->UpdateCurrentAtk(IncreaseAtkValue);
			break;
		case EINFStackItem::INCREASE_MAXHP:
			ManagerComp->UpdateMaxHP(IncreaseMaxHpValue);
			break;
		}
	}
}

bool UItemComponent::ApplyFixMaxItem(EFixMaxStackItem Item)
{
	if (ManagerComp)
	{
		switch (Item)
		{
			case EFixMaxStackItem::INCREASE_RANGE:
			{
				uint8 Value = GetFixMaxStack(EFixMaxStackItem::INCREASE_RANGE);
				if (Value < FIX_MAX_STACK)
				{
					IncreaseFixMaxStack(EFixMaxStackItem::INCREASE_RANGE);
					ManagerComp->UpdateCurrentRange(50.f);
					return true;
				}
				return false;
			}
			case EFixMaxStackItem::INCREASE_CRITICAL_PER:
			{
				uint8 Value = GetFixMaxStack(EFixMaxStackItem::INCREASE_CRITICAL_PER);
				if (Value < FIX_MAX_STACK)
				{
					IncreaseFixMaxStack(EFixMaxStackItem::INCREASE_CRITICAL_PER);
					ManagerComp->UpdateCurrentCritical(1.f);
					return true;
				}
				return false;
			}
		}
	}
	return false;
}

bool UItemComponent::ApplyOnceEquipItem(FItemInfoTable* Item, OUT EOnceEquipItemFlag& Flag)
{
	
	if (CheckOnceItem(static_cast<uint8>(Item->DetailType.OnceEquipItem)))
	{
		Flag = EOnceEquipItemFlag::EQUIPPED_ALREADY_THIS_ITEM;
		return true;
	}
	else
	{
		if (ItemManager.EquippedItemCount < 2)
		{
			ApplyOnceItem(static_cast<uint8>(Item->DetailType.OnceEquipItem));
			ItemManager.EquippedItemCount++;
			Flag = EOnceEquipItemFlag::SUCCESS;
			return true;
		}
		else
		{
			Flag = EOnceEquipItemFlag::EQUIPPED_TWO_OTHER_ITEMS;
			
			return false;
		}
	}
	return false;
}

uint8 UItemComponent::GetFixMaxStack(EFixMaxStackItem Item)
{
	uint8* Ret = FixMaxNum.Find(static_cast<uint8>(Item));
	if (Ret)
	{
		return *Ret;
	}
	else
	{
		FixMaxNum.Add(static_cast<uint8>(Item), 0);
	}
	return 0;
}

void UItemComponent::IncreaseFixMaxStack(EFixMaxStackItem Item)
{
	uint8* Ret = FixMaxNum.Find(static_cast<uint8>(Item));
	if (Ret)
	{
		*Ret = FMath::Clamp(*Ret + 1, 0, 10);
	}
}

void UItemComponent::ItemSwap(const FItemInfoTable* OldItem, const FItemInfoTable* NewItem)
{
	if (OldItem && NewItem)
	{
		RemoveOnceItem(static_cast<uint8>(OldItem->DetailType.OnceEquipItem));
		ApplyOnceItem(static_cast<uint8>(NewItem->DetailType.OnceEquipItem));
	}
}