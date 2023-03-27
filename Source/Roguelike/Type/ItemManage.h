#pragma once
#include "Engine/DataTable.h"
#include "ItemManage.generated.h"

class UItemInfo;
UENUM()
enum class EItemType : uint8 //아이템 종류
{
	None,
	InfStackItem,
	FixMaxStackItem,
	OnceEquipItem,

	MAX
};

UENUM()
enum class EINFStackItem : uint8
{
	None = 0,
	IncreaseAtk,
	IncreaseMaxHp,

	MAX
};

UENUM()
enum class EFixMaxStackItem : uint8
{
	None = 0,
	IncreaseRange,
	IncreaseCriticalPer,

	MAX
};


UENUM(Meta = (Bitflags))
enum class EOnceEquippedItem : uint8
{
	Default = 0,
	RiskReturn = 1 << 0,
	MultiShot = 1 << 1,
	Dodge = 1 << 2,
	Triple = 1 << 3,

	MAX
};
ENUM_CLASS_FLAGS(EOnceEquippedItem);

USTRUCT(BlueprintType)
struct FItemType
{
	GENERATED_BODY()
	FItemType():
		INFStackItem(EINFStackItem::None), 
		FixMaxStackItem(EFixMaxStackItem::None),
		OnceEquipItem(EOnceEquippedItem::Default) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EINFStackItem INFStackItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFixMaxStackItem FixMaxStackItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOnceEquippedItem OnceEquipItem;

	friend FArchive& operator<<(FArchive& Ar, FItemType& Type)
	{
		Ar << Type.INFStackItem;
		Ar << Type.FixMaxStackItem;
		Ar << Type.OnceEquipItem;
		
		return Ar;
	}

};


USTRUCT()
struct FItemManager
{
	GENERATED_BODY()
	FItemManager() : OnceEquippedItem(0), EquippedItemCount(0) {}

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = EOnceEquippedItem))
	uint8 OnceEquippedItem;
	UPROPERTY(VisibleAnywhere)
	int8 EquippedItemCount;

	friend FArchive& operator<<(FArchive& Ar, FItemManager& ItemManager)
	{
		Ar << ItemManager.OnceEquippedItem;
		Ar << ItemManager.EquippedItemCount;
		
		return Ar;
	}
};

UCLASS()
class UItemInfo : public UObject
{
	GENERATED_BODY()
public:
	UItemInfo() :
		ItemsType(EItemType::None),
		DetailType(FItemType()),
		ItemName(FString()),
		ItemDesc(FString()),
		ItemIcon(nullptr) {}
	
	EItemType ItemsType;
	FItemType DetailType;
	FString ItemName;
	FString ItemDesc;
	UTexture2D* ItemIcon;
	bool HaveTooltip;
	FString TooltipText;

	static UItemInfo* ConstructItemInfo(EItemType InItemType, FItemType InDetailType, FString InItemName, FString InItemDesc, UTexture2D* InItemIcon, bool InHaveTooltip, FString InTooltipText)
	{
		UItemInfo* Info = NewObject<UItemInfo>();
		Info->Init(InItemType, InDetailType, InItemName, InItemDesc, InItemIcon, InHaveTooltip, InTooltipText);
		return Info;
	}


	friend FArchive& operator<<(FArchive& Ar, UItemInfo& Info)
	{
		Ar << Info.ItemsType;
		Ar << Info.DetailType;
		Ar << Info.ItemName;
		Ar << Info.ItemDesc;
		Ar << Info.ItemIcon;
		Ar << Info.HaveTooltip;
		Ar << Info.TooltipText;
		
		return Ar;
	}

private:
	void Init(EItemType InItemType, FItemType InDetailType, FString InItemName, FString InItemDesc, UTexture2D* InItemIcon, bool InHaveTooltip, FString InTooltipText)
	{
		ItemsType = InItemType;
		DetailType = InDetailType;
		ItemName = InItemName;
		ItemDesc = InItemDesc;
		ItemIcon = InItemIcon;
		HaveTooltip = InHaveTooltip;
		TooltipText = InTooltipText;
	}
};



USTRUCT(BlueprintType)
struct FItemInfoTable : public FTableRowBase
{
	GENERATED_BODY()
		FItemInfoTable() :
		ItemsType(EItemType::None), 
		DetailType(FItemType()),
		ItemName(FString()),
		ItemDesc(FString()),
		ItemIcon(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemsType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemType DetailType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HaveTooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TooltipText;

};


enum class EOnceEquipItemFlag
{
	Success,
	EquippedAlreadyThisItem,
	EquippedTwoOtherItems
};


UCLASS()
class ROGUELIKE_API UItemManage : public UObject
{
	GENERATED_BODY()
};