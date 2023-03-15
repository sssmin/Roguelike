#pragma once
#include "StatManage.generated.h"

UENUM()
enum class EElement : uint8
{
	None UMETA(DisplayName = "NONE"),
	Fire UMETA(DisplayName = "FIRE"),
	Water UMETA(DisplayName = "WATER"),
	Earth UMETA(DisplayName = "EARTH"),
	Darkness UMETA(DisplayName = "DARKNESS"),
	Light UMETA(DisplayName = "LIGHT"),

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

	friend FArchive& operator<<(FArchive& Ar, FHealthManager& HealthManager)
	{
		Ar << HealthManager.MaxHP;
		Ar << HealthManager.CurrentHP;
		return Ar;
	}
};

USTRUCT(BlueprintType)
struct FCombatManager
{
	GENERATED_BODY()
	FCombatManager() : ATK(5.f), Element(EElement::None), Critical(0.f), Range(900.f) {}
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

	friend FArchive& operator<<(FArchive& Ar, FCombatManager& CombatManager)
	{
		Ar << CombatManager.ATK;
		Ar << CombatManager.Element;
		Ar << CombatManager.Critical;
		Ar << CombatManager.Range;
		
		return Ar;
	}
};


UENUM(Meta = (Bitflags))
enum class EState : uint8
{
	Default			= 0,
	Burn			= 1 << 0,
	Frozen			= 1 << 1, 
	Poison			= 1 << 2,
	Fear			= 1 << 3, 
	Dead			= 1 << 4, 

	MAX
};
ENUM_CLASS_FLAGS(EState);

UENUM(Meta = (Bitflags))
enum class EBuff : uint8
{
	Default			= 0,
	MovementBuff	= 1 << 0,
	AtkBuff		= 1 << 1,
	HealBuff		= 1 << 2,
	
	MAX
};
ENUM_CLASS_FLAGS(EBuff);



UCLASS()
class ROGUELIKE_API UStatManager : public UObject
{
	GENERATED_BODY()
};