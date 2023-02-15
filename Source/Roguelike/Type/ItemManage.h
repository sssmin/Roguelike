#pragma once
#include "Engine/DataTable.h"
#include "ItemManage.generated.h"

UENUM()
enum class EItemType : uint8 //아이템 종류
{
	NONE,
	INF_STACK_ITEM, 
	FIX_MAX_STACK_ITEM, 
	ONCE_EQUIP_ITEM, 

	MAX
};

UENUM()
enum class EINFStackItem : uint8
{
	NONE = 0,
	INCREASE_ATK,
	INCREASE_MAXHP,

	MAX
};

UENUM()
enum class EFixMaxStackItem : uint8
{
	NONE = 0,
	INCREASE_RANGE,
	INCREASE_CRITICAL_PER,

	MAX
};


UENUM(Meta = (Bitflags))
enum class EOnceEquippedItem : uint8
{
	DEFAULT = 0,
	RISK_RETURN = 1 << 0,
	MULTI_SHOT = 1 << 1,
	DODGE = 1 << 2,

	MAX
};
ENUM_CLASS_FLAGS(EOnceEquippedItem);

USTRUCT(BlueprintType)
struct FItemType
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EINFStackItem INFStackItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFixMaxStackItem FixMaxStackItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EOnceEquippedItem OnceEquipItem;

};


USTRUCT()
struct FItemManager
{
	GENERATED_BODY()
	FItemManager() : OnceEquippedItem(0), EquippedItemCount(0) {}

	UPROPERTY(Meta = (Bitmask, BitmaskEnum = EOnceEquippedItem))
	uint8 OnceEquippedItem;

	int8 EquippedItemCount; //2개까지
};

UCLASS()
class UItemInfo : public UObject
{
	GENERATED_BODY()
public:
	UItemInfo() {}
	
	EItemType ItemType;

	FItemType DetailType;

	FString ItemName;

	FString ItemDesc;

	UPROPERTY()
	UTexture2D* ItemIcon;

	static UItemInfo* ConstructItemInfo(EItemType InItemType, FItemType InDetailType, FString InItemName, FString InItemDesc, UTexture2D* InItemIcon)
	{
		UItemInfo* Info = NewObject<UItemInfo>();
		Info->Init(InItemType, InDetailType, InItemName, InItemDesc, InItemIcon);
		return Info;
	}

private:
	void Init(EItemType InItemType, FItemType InDetailType, FString InItemName, FString InItemDesc, UTexture2D* InItemIcon)
	{
		ItemType = InItemType;
		DetailType = InDetailType;
		ItemName = InItemName;
		ItemDesc = InItemDesc;
		ItemIcon = InItemIcon;
	}
};



USTRUCT(BlueprintType)
struct FItemInfoTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemType DetailType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

};


enum class EOnceEquipItemFlag
{
	SUCCESS,
	EQUIPPED_ALREADY_THIS_ITEM,
	EQUIPPED_TWO_OTHER_ITEMS
};


UCLASS()
class ROGUELIKE_API UItemManage : public UObject
{
	GENERATED_BODY()
};