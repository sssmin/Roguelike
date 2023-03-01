#pragma once
#include "StatManage.generated.h"

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

USTRUCT(BlueprintType)
struct FHealthManager
{
	GENERATED_BODY()
	FHealthManager() : MaxHP(100.f), CurrentHP(100.f) {}
	FHealthManager(float InMaxHP) :
		MaxHP(InMaxHP), CurrentHP(InMaxHP) {}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentHP;
};

USTRUCT(BlueprintType)
struct FCombatManager
{
	GENERATED_BODY()
	FCombatManager() : ATK(5.f), Element(EElement::NONE), Critical(0.f), Range(900.f) {}
	FCombatManager(float InATK, EElement InElement) :
		ATK(InATK), Element(InElement), Critical(0.f), Range(900.f) {}


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ATK;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EElement Element;
	UPROPERTY(VisibleAnywhere)
	float Critical;
	UPROPERTY(VisibleAnywhere)
	float Range;
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



UCLASS()
class ROGUELIKE_API UStatManager : public UObject
{
	GENERATED_BODY()
};