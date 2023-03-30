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
}

void UItemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}

void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI && GI->GetListenerManager())
	{
		GI->InitOnceItemDelegate.BindUObject(this, &ThisClass::InitEquipItems);
		GI->GetListenerManager()->OnSelectItemDelegate.BindUObject(this, &ThisClass::SelectItem);
		GI->SetTempManageDelegate.AddUObject(this, &ThisClass::SetTempManager);
		GI->GetListenerManager()->OnLoadGameDelegate.AddUObject(this, &ThisClass::Init);
	}
}

void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UItemComponent::Init()
{
	if (URLGameInstance* GI = URLGameInstance::GetRLGameInst(this))
	{
		GI->GetManager(ItemManager, FixMaxNum, ItemInfos);
	}
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
		case EINFStackItem::IncreaseAtk:
			{
				const TArray<float> Per = { 3.f, 6.f, 9.f, 10.f, 12.f, 30.f, 40.f };
				const TArray<int32> Value = { 9, 8, 7, 6, 5, 3, 2 };
				const float AddToValue = CalcStatValuePer(Per, Value);
				ManagerComp->UpdateCurrentAtk(AddToValue);
			}
			break;
		case EINFStackItem::IncreaseMaxHp:
			{
				const TArray<float> Per = { 3.f, 6.f, 9.f, 10.f, 12.f, 30.f, 40.f };
				const TArray<int32> Value = { 28, 24, 21, 18, 15, 12, 10 };
				const float AddToValue = CalcStatValuePer(Per, Value);
				ManagerComp->UpdateMaxHP(AddToValue);
				ManagerComp->HealByValue(AddToValue);
			}
			break;
		}
	}
}

float UItemComponent::CalcStatValuePer(const TArray<float>& Per, const TArray<int32>& Value)
{
	for (int32 i = 0; i < Per.Num() + 1; ++i)
	{
		const float RandValue = FMath::Rand() % 100 + 1;
		float AddValue = 0.f;
		for (int32 j = 0; j < Per.Num(); ++j)
		{
			AddValue += Per[j];
			if (RandValue <= AddValue)
			{
				return Value[j];
			}
		}
	}
	return 0.f;
}

bool UItemComponent::ApplyFixMaxItem(EFixMaxStackItem Item)
{
	if (ManagerComp)
	{
		switch (Item)
		{
			case EFixMaxStackItem::IncreaseRange:
			{
				const uint8 Value = GetFixMaxStack(EFixMaxStackItem::IncreaseRange);
				if (Value < FIX_MAX_STACK)
				{
					IncreaseFixMaxStack(EFixMaxStackItem::IncreaseRange);
					ManagerComp->UpdateCurrentRange(50.f);
					return true;
				}
				return false;
			}
			case EFixMaxStackItem::IncreaseCriticalPer:
			{
				const uint8 Value = GetFixMaxStack(EFixMaxStackItem::IncreaseCriticalPer);
				if (Value < FIX_MAX_STACK)
				{
					IncreaseFixMaxStack(EFixMaxStackItem::IncreaseCriticalPer);
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
			Flag = EOnceEquipItemFlag::EquippedAlreadyThisItem;
			return true;
		}
		else
		{
			if (ItemManager.EquippedItemCount < 2)
			{
				ApplyOnceItem(static_cast<uint8>(Item->DetailType.OnceEquipItem));
				ItemManager.EquippedItemCount++;
				Flag = EOnceEquipItemFlag::Success;
				return true;
			}
			else
			{
				Flag = EOnceEquipItemFlag::EquippedTwoOtherItems;
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

void UItemComponent::ItemSwap(UItemInfo* OldItem, UItemInfo* NewItem)
{
	if (OldItem && NewItem)
	{
		RemoveOnceItem(static_cast<uint8>(OldItem->DetailType.OnceEquipItem));
		ApplyOnceItem(static_cast<uint8>(NewItem->DetailType.OnceEquipItem));
		int32 Idx = ItemInfos.Find(OldItem);
		if (Idx != INDEX_NONE)
		{
			ItemInfos[Idx] = NewItem;
		}
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
		case EItemType::InfStackItem:
		{
			ApplyInfItem(Item->DetailType.INFStackItem);
			ResumeController(RLPC);
			break;
		}
		case EItemType::FixMaxStackItem:
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
		case EItemType::OnceEquipItem:
		{
			EOnceEquipItemFlag Flag;
			bool Ret = ApplyOnceEquipItem(Item, Flag);
			if (Ret)
			{
				switch (Flag)
				{
				case EOnceEquipItemFlag::Success:
					ResumeController(RLPC);
					RLPC->RegisterItemEmptySlot(Item);
					ItemInfos.Add(Item);
					break;
				case EOnceEquipItemFlag::EquippedAlreadyThisItem:
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
	ItemInfos.Empty();
	ARLPlayerController* RLPC = Cast<ARLPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (RLPC)
	{
		RLPC->InitOnceItemWidget();
	}
}

void UItemComponent::SetTempManager() const
{
	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI)
	{
		GI->SetManager(ItemManager, FixMaxNum, ItemInfos);
	}
}