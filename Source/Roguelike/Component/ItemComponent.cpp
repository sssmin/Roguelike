// Fill out your copyright notice in the Description page of Project Settings.
#include "ItemComponent.h"
#include "Kismet/GameplayStatics.h"

#include "ManagerComponent.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/Game/RLListenerManager.h"


UItemComponent::UItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	ItemManager = FItemManager();
	IncreaseAtkValue = 3.f;
	IncreaseMaxHpValue = 15.f;

}

void UItemComponent::InitializeComponent()
{
	Super::InitializeComponent();

	
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		GI->GetManager(ItemManager, FixMaxNum);
		GI->InitOnceItemDelegate.BindUObject(this, &ThisClass::InitEquipItems);
		GI->GetListenerManager()->OnSelectItemDelegate.BindUObject(this, &ThisClass::SelectItem);
	}
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

bool UItemComponent::ApplyOnceEquipItem(const UItemInfo* Item, OUT EOnceEquipItemFlag& Flag)
{
	if (Item)
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
				ItemIcons.Add(static_cast<uint8>(Item->DetailType.OnceEquipItem), Item->ItemIcon);
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

void UItemComponent::ItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem)
{
	if (OldItem && NewItem)
	{
		RemoveOnceItem(static_cast<uint8>(OldItem->DetailType.OnceEquipItem));
		ApplyOnceItem(static_cast<uint8>(NewItem->DetailType.OnceEquipItem));
		ItemIcons.Remove(static_cast<uint8>(OldItem->DetailType.OnceEquipItem));
		ItemIcons.Add(static_cast<uint8>(NewItem->DetailType.OnceEquipItem), NewItem->ItemIcon);
	}
}

void UItemComponent::SendManager()
{
	URLGameInstance* GI = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		GI->SetManager(ItemManager, FixMaxNum);
	}
}

void UItemComponent::ResumeController(ARLPlayerController* RLPC)
{
	check(RLPC);
	RLPC->RemoveSelectWidget();
	RLPC->ResumeController();
}

void UItemComponent::SelectItem(UItemInfo* Item)
{
	ARLPlayerController* RLPC = Cast<ARLPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	check(RLPC);
	checkf(Item, TEXT("Item is nullptr"));

	RLPC->DeactivateOnceItemListWidget();
	switch (Item->ItemsType)
	{
		case EItemType::INF_STACK_ITEM:
		{
			ApplyInfItem(Item->DetailType.INFStackItem);
			ResumeController(RLPC);
			//이펙트, 소리
			break;
		}
		case EItemType::FIX_MAX_STACK_ITEM:
		{
			bool Ret = ApplyFixMaxItem(Item->DetailType.FixMaxStackItem);
			if (!Ret)
			{
				RLPC->ShowNoticeWidget(TEXT("이미 10번 증가한 스탯이에요."));
				return;
			}
			ResumeController(RLPC);
			break;
		}
		case EItemType::ONCE_EQUIP_ITEM:
		{
			EOnceEquipItemFlag Flag;
			bool Ret = ApplyOnceEquipItem(Item, Flag);
			if (Ret)
			{
				switch (Flag)
				{
				case EOnceEquipItemFlag::SUCCESS:
					ResumeController(RLPC);
					RLPC->RegisterItemEmptySlot(Item);
					break;
				case EOnceEquipItemFlag::EQUIPPED_ALREADY_THIS_ITEM:
					RLPC->ShowNoticeWidget(TEXT("이미 장착되어 있는 아이템이에요."));
					break;
				}
			}
			else //아이템 창 2개 중 선택
			{
				RLPC->ActiveOnceItemListWidget(Item);
			}
			break;
		}
	}
}

void UItemComponent::InitEquipItems()
{
	ItemManager.OnceEquippedItem = 0;
	ItemManager.EquippedItemCount = 0;
	ARLPlayerController* RLPC = Cast<ARLPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (RLPC)
	{
		RLPC->InitOnceItemWidget();
	}
}