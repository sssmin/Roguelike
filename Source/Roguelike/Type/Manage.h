#pragma once
#include "Manage.generated.h"

UENUM()
enum class EElement : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	FIRE UMETA(DisplayName = "FIRE"),
	WATER UMETA(DisplayName = "WATER"),
	EARTH UMETA(DisplayName = "EARTH"),
	DARKNESS UMETA(DisplayName = "DARKNESS"),
	LIGHT UMETA(DisplayName = "LIGHT"),

	MAX
};

USTRUCT()
struct FHealthManage
{
	GENERATED_BODY()
	FHealthManage() : MaxHP(100.f), CurrentHP(100.f) {}
	FHealthManage(float InMaxHP) :
		MaxHP(InMaxHP), CurrentHP(InMaxHP) {}

	UPROPERTY(VisibleAnywhere)
	float MaxHP;
	UPROPERTY(VisibleAnywhere)
	float CurrentHP;
};

USTRUCT()
struct FCombatManage
{
	GENERATED_BODY()
	FCombatManage() : ATK(5.f), Element(EElement::NONE), Delay(2.f) {}
	FCombatManage(float InATK, EElement InElement, float InDelay) :
		ATK(InATK), Element(InElement), Delay(2.f) {}


	UPROPERTY(VisibleAnywhere)
	float ATK;
	UPROPERTY(VisibleAnywhere)
	EElement Element;
	UPROPERTY(VisibleAnywhere)
	float Delay;

	//사거리 추가
};

USTRUCT()
struct FItemManage
{
	GENERATED_BODY()
	FItemManage() : OnceEquipedItem(0), EquippedItemCount(0) {}
	
	UPROPERTY(Meta = (Bitmask, BitmaskEnum = EOnceEquippedItem))
	uint8 OnceEquipedItem;

	int32 EquippedItemCount; //2개까지
};

UENUM(Meta = (Bitflags))
enum class EState : uint8
{
	DEFAULT			= 0,
	BURN			= 1 << 0,
	FROZEN			= 1 << 1, 
	POISON			= 1 << 2,
	FEAR			= 1 << 3, 
	DEAD			= 1 << 4, 

	MAX
};
ENUM_CLASS_FLAGS(EState);

UENUM(Meta = (Bitflags))
enum class EBuff : uint8
{
	DEFAULT			= 0,
	MOVEMENT_BUFF	= 1 << 0,
	ATK_BUFF		= 1 << 1,
	HEAL_BUFF		= 1 << 2,
	
	MAX
};
ENUM_CLASS_FLAGS(EBuff);


UENUM(Meta = (Bitflags))
enum class EOnceEquippedItem : uint8
{
	DEFAULT			= 0,
	RISK_RETURN		= 1 << 0,
	MULTI_SHOT		= 1 << 1,
	

	MAX
};
ENUM_CLASS_FLAGS(EOnceEquippedItem);


UCLASS()
class ROGUELIKE_API UManager : public UObject
{
	GENERATED_BODY()
};