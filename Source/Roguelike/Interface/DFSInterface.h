// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Type/Types.h"
#include "UObject/Interface.h"
#include "DFSInterface.generated.h"

USTRUCT(BlueprintType)
struct FCell
{
	GENERATED_BODY()
	FCell()
	{
		Visited = false;
		Status.Init(false, 4);
		CellState = ECellState::DEACTIVATE;
		CellType = ECellType::NONE;
		IsCleared = false;
		TempWall = 0;
		CellClass = 2;
		SelectedBonusItem = false;
	}
	bool Visited;
	TArray<bool> Status;
	ECellState CellState;
	ECellType CellType;
	bool IsCleared;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = ETempWall))
	uint8 TempWall;
	int32 CellClass;
	bool SelectedBonusItem;
};

USTRUCT()
struct FVector2Int
{
	GENERATED_BODY()

	FVector2Int() : X(0), Y(0) {}
	FVector2Int(int32 InX, int32 InY) : X(InX), Y(InY) {}

	UPROPERTY(EditAnywhere)
	int32 X;
	UPROPERTY(EditAnywhere)
	int32 Y;

};

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValueAsMaskValuesInEditor = "true"))
enum class ETempWall : uint8
{
	NONE			= 0,
	CENTER_UP		= 1 << 0,
	CENTER_DOWN		= 1 << 1,
	CENTER_RIGHT	= 1 << 2,
	CENTER_LEFT		= 1 << 3,
	SIDE_UP			= 1 << 4,
	SIDE_DOWN		= 1 << 5,
	SIDE_RIGHT		= 1 << 6,
	SIDE_LEFT		= 1 << 7
};
ENUM_CLASS_FLAGS(ETempWall);


UINTERFACE(MinimalAPI)
class UDFSInterface : public UInterface
{
	GENERATED_BODY()
};

class ROGUELIKE_API IDFSInterface
{
	GENERATED_BODY()

public:
};
