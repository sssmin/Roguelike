#pragma once
#include "Manage.generated.h"

UENUM()
enum class EElement : uint8
{
	NONE,
	FIRE,
	WATER,
	EARTH,
	DARKNESS,
	LIGHT,

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
	FCombatManage() : ATK(5.f), Element(EElement::NONE) {}
	FCombatManage(float InATK, EElement InElement) :
		ATK(InATK), Element(InElement) {}


	UPROPERTY(VisibleAnywhere)
	float ATK;
	UPROPERTY(VisibleAnywhere)
	EElement Element;
};

UENUM(Meta = (Bitflags))
enum class EState : uint8
{
	DEFAULT			= 0,
	FROZEN			= 1 << 0, 
	MOVEMENTBUFF	= 1 << 1, 
	FEAR			= 1 << 2, 
	FIRESTACK		= 1 << 3, 
	DEAD			= 1 << 4, 

	MAX
};
ENUM_CLASS_FLAGS(EState);

UCLASS()
class ROGUELIKE_API UManager : public UObject
{
	GENERATED_BODY()
};