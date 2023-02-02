// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "UObject/Interface.h"
#include "DFSInterface.generated.h"


struct FCell
{
	FCell()
	{
		Visited = false;
		Status.Init(false, 4);
		CellState = ECellState::DEACTIVE;
		CellType = ECellType::NONE;
		IsCleared = false;
	}
	bool Visited;
	TArray<bool> Status;
	ECellState CellState;
	ECellType CellType;
	bool IsCleared;
};

USTRUCT()
struct FVector2Int
{
	GENERATED_BODY()

	FVector2Int() {}
	FVector2Int(int32 InX, int32 InY) :X(InX), Y(InY) {}

	UPROPERTY(EditAnywhere)
	int32 X;
	UPROPERTY(EditAnywhere)
	int32 Y;

};

USTRUCT()
struct FRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Room;
	UPROPERTY(EditAnywhere)
	FVector2Int MinPosition;
	UPROPERTY(EditAnywhere)
	FVector2Int MaxPosition;
	UPROPERTY(EditAnywhere)
	bool Obligatory;

	int ProbilityOfSpawning(int32 X, int32 Y)
	{
		if (X >= MinPosition.X && X <= MaxPosition.X && Y >= MinPosition.Y && Y <= MaxPosition.Y)
		{
			return Obligatory ? 2 : 1;
		}

		return 0;
	}
};


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
