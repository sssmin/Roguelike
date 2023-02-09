#pragma once
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM()
enum class EItemType : uint8 //아이템 종류
{
	NONE,
	ELEMENT,
	INF_STACK_ITEM, //Stack 2개는 바로 적용
	FIX_MAX_STACK_ITEM, //바로 적용, 보유 인데 최대치 있음
	ONCE_EQUIP_ITEM, //얘는 보유

	MAX
};

enum class EINFStackItem : uint8
{
	NONE = 0,
	INCREASE_ATK,
	INCREASE_MAXHP,

	MAX
};

enum class EFixMaxStackItem : uint8
{
	NONE = 0,
	INCREASE_RANGE,
	DECREASE_DELAY,

	MAX
};

enum class EOnceEquipItem : uint8
{
	DEFAULT = 0,
	RIST_RETURN,
	MULTI_SHOT,


	MAX
};


USTRUCT(BlueprintType)
struct FAllItemTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnumNum;

};

UCLASS()
class ROGUELIKE_API UItem : public UObject
{
	GENERATED_BODY()
};