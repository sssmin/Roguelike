// Fill out your copyright notice in the Description page of Project Settings.

#include "DFSAgrt.h"

DFSAgrt::DFSAgrt()
{
	ValueInit();
}

DFSAgrt::~DFSAgrt()
{

}

void DFSAgrt::StartAlgorithm(FVector2Int MapSize)
{
	Size = MapSize;
	ValueInit(); //ÃÊ±â°ª ¼¼ÆÃ
	Init(); //¼¼ÆÃ ÇÑ °ª ÃÊ±âÈ­
	MazeGenerator();
	MakeBonusCell();
}

void DFSAgrt::ValueInit()
{
	Board.Empty();
	CellCount = 0;
	StartCell = 0;
	Parts = 1;
	CreatedPart = 0;
	BossCell = 0;
	BossPrevCell = 0;
	StartPostion = FVector::ZeroVector;
	TotalCellNum = 0;
	BonusCellNum = 0;
	CreatedBossCell = false;
}

void DFSAgrt::Init()
{
	for (int32 i = 0; i < Size.X * Size.Y; ++i)
	{
		Board.Add(FCell());
	}

	CellCount = (Size.X + Size.Y) / 2;
	Parts = FMath::RandRange(1, 3);
	if (Parts == 1)
		CellCount += CellCount / 2;

	BonusCellNum = FMath::RandRange(1, Parts);
	TotalCellNum -= BonusCellNum;

	//CellCount = 3;
	//Parts = 2; 
	//BonusCellNum = 1; //test

	int32 Length = Size.X / 2;
	int32 Breadth = Size.Y / 2;
	for (int32 y = 0; y < Breadth; ++y)
	{
		StartCell += Size.Y;
	}
	StartCell += Length;
}


void DFSAgrt::MazeGenerator()
{
	int32 CurrentCell = StartCell;

	TArray<int> Path;
	TArray<int32> SideCells;

	int32 CreatedCellCount = 0;
	int32 BeforeCell = 0;

	while (true)
	{
		SideCells.Empty();

		Board[CurrentCell].Visited = true;
		Board[CurrentCell].CellState = ECellState::NORMAL;
		Board[CurrentCell].CellType = ECellType::MOBS;
		Board[CurrentCell].CellClass = FMath::RandRange(2, 4);
		for (int32 i = 0; i < 8; ++i)
		{
			int32 Rand = FMath::RandRange(1, 4);
			if (Rand == 1)
			{
				Board[CurrentCell].TempWall |= 1 << i;
			}
		}
		
		if (CreatedCellCount == CellCount) break;

		if (CurrentCell == Board.Num()) break;

		SideCells = CheckSideCell(CurrentCell);

		if (SideCells.Num() != 0)
		{
			BeforeCell = CurrentCell;
			CreatedCellCount++;
			TotalCellNum++;

			int32 NewCell = SideCells[FMath::RandRange(0, SideCells.Num() - 1)];

			if (NewCell > CurrentCell) //ÀÌ¿ô ¼¿ÀÌ ÇöÀç ¼¿º¸´Ù Å©¸é? ¾Æ·¡ ¾Æ´Ï¸é ¿À¸¥ÂÊ ¼¿ÀÌ´Ù.
			{
				if (NewCell - 1 == CurrentCell) //ÀÌ¿ô ¼¿Àº ¿À¸¥ÂÊ ¼¿
				{
					Board[CurrentCell].Status[2] = true;   //0 À§ 1 ¾Æ·¡ 2 ¿À¸¥ 3 ¿Þ
					CurrentCell = NewCell;
					Board[CurrentCell].Status[3] = true;
				}
				else //ÀÌ¿ô ¼¿Àº ¾Æ·¡ ¼¿
				{
					Board[CurrentCell].Status[1] = true;
					CurrentCell = NewCell;
					Board[CurrentCell].Status[0] = true;
				}
			}
			else //ÀÌ¿ô ¼¿Àº À§ ¾Æ´Ï¸é ¿ÞÂÊ ¼¿ÀÌ´Ù.
			{
				if (NewCell + 1 == CurrentCell) //ÀÌ¿ô ¼¿Àº ¿ÞÂÊ ¼¿
				{
					Board[CurrentCell].Status[3] = true;
					CurrentCell = NewCell;
					Board[CurrentCell].Status[2] = true;
				}
				else //ÀÌ¿ô ¼¿Àº À§ ¼¿
				{
					Board[CurrentCell].Status[0] = true;
					CurrentCell = NewCell;
					Board[CurrentCell].Status[1] = true;
				}
			}
		}
		else
		{
			break;
		}
	}

	if (!CreatedBossCell)
	{
		CreatedBossCell = true;
		BossPrevCell = BeforeCell;
		BossCell = CurrentCell;
		Board[BossCell].CellState = ECellState::DONT_FIND_BOSS;
		Board[BossCell].CellType = ECellType::BOSS;
		Board[BossCell].CellClass = 1;
	}
	
	if (++CreatedPart != Parts)
	{
		MazeGenerator();
	}
	else
	{
		Board[StartCell].CellState = ECellState::IN_PLAYER;
		Board[StartCell].CellType = ECellType::START;
		Board[StartCell].IsCleared = true;
		Board[StartCell].TempWall = static_cast<uint8>(ETempWall::NONE);
		Board[StartCell].CellClass = 0;
	}
}

void DFSAgrt::MakeBonusCell()
{
	int32 MakedBonusCell = 0;
	int32 RandCellIdx = 0;

	while (MakedBonusCell != BonusCellNum)
	{
		RandCellIdx = FMath::RandRange(0, (Size.X * Size.Y) - 1);
		if (Board[RandCellIdx].Visited && (RandCellIdx != BossCell) && (RandCellIdx != BossPrevCell) && (RandCellIdx != StartCell))
		{
			Board[RandCellIdx].CellState = ECellState::BONUS;
			Board[RandCellIdx].CellType = ECellType::BONUS;
			Board[RandCellIdx].TempWall = static_cast<uint8>(ETempWall::NONE);
			Board[RandCellIdx].CellClass = 0;
			MakedBonusCell++;
		}
	}
}


TArray<int32> DFSAgrt::CheckSideCell(int32 Cell)
{
	TArray<int32> SideCells;

	if (Cell - Size.Y >= 0 && !Board[Cell - Size.Y].Visited) 
	{
		SideCells.Add(Cell - Size.Y);
	}
	if (Cell + Size.Y < Board.Num() && !Board[Cell + Size.Y].Visited)
	{
		SideCells.Add(Cell + Size.Y);
	}
	if ((Cell + 1) % Size.Y != 0 && !Board[Cell + 1].Visited) 
	{
		SideCells.Add(Cell + 1); 
	}
	if (Cell % Size.X != 0 && !Board[Cell - 1].Visited)
	{
		SideCells.Add(Cell - 1); 
	}

	return SideCells;
}

void DFSAgrt::GetMaxWidgetHeight(OUT int32 MaxWidth, OUT int32 MaxHeight)
{
	for (int32 i = 0; i < Size.Y; ++i)
	{
		int32 Compare = 0;
		for (int32 j = 0; j < Size.X; j++)
		{
			FCell CurrentCell = Board[i * Size.Y + j]; //0 1 2 3 4 ¼ø
			if (CurrentCell.Visited)
			{
				Compare++;
			}
		}
		MaxWidth = FMath::Max(Compare, MaxWidth);
	}


	for (int32 i = 0; i < Size.X; ++i)
	{
		int32 Compare = 0;
		for (int32 j = 0; j < Size.Y; ++j)
		{
			FCell CurrentCell = Board[i + j * Size.X]; //0 5 10 15 ¼ø
			if (CurrentCell.Visited)
			{
				Compare++;
			}
		}
		MaxHeight = FMath::Max(Compare, MaxHeight);
	}
	
}
