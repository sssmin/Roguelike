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
	GenerateDungeon();
}

void DFSAgrt::ValueInit()
{
	Board.Empty();
	CellCount = 0;
	StartPos = 0;
	Parts = 1;
	CreatedPart = 0;
	BossCell = 0;
	BossPrevCell = 0;
	StartPostion = FVector::ZeroVector;
	TotalCellNum = 0;
}

void DFSAgrt::Init()
{
	for (int i = 0; i < Size.X * Size.Y; ++i)
	{
		Board.Add(FCell());
	}

	CellCount = (Size.X + Size.Y) / 2;
	Parts = FMath::RandRange(1, 3);
	if (Parts == 1)
		CellCount += CellCount / 2;

	int32 Length = Size.X / 2;
	int32 Breadth = Size.Y / 2;
	for (int y = 0; y < Breadth; ++y)
	{
		StartPos += Size.Y;
	}
	StartPos += Length;
}


void DFSAgrt::MazeGenerator()
{
	int32 CurrentCell = StartPos;

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
	

	if (++CreatedPart != Parts)
	{
		MazeGenerator();
	}
	else
	{
		BossPrevCell = BeforeCell;
		BossCell = CurrentCell;
		Board[BossCell].CellState = ECellState::DONTFINDBOSS;
		Board[BossCell].CellType = ECellType::BOSS;
		Board[StartPos].CellState = ECellState::INPLAYER;
		Board[StartPos].CellType = ECellType::START;
		Board[StartPos].IsCleared = true;
	}
}

void DFSAgrt::GenerateDungeon()
{
	//for (int i = 0; i < Size.X; ++i)
	//{
	//	for (int j = 0; j < Size.Y; ++j)
	//	{
	//		FCell CurrentCell = Board[i + j * Size.X];
	//		if (CurrentCell.Visited)
	//		{
	//			int32 RandomRoom = -1;
	//			TArray<int32> AvaliableRooms;
	//			for (int k = 0; k < Rooms.Num(); ++k)
	//			{
	//				int32 P = Rooms[k].ProbilityOfSpawning(i, j);

	//				if (P == 2)
	//				{
	//					RandomRoom = k;
	//					break;
	//				}
	//				else if (P == 1)
	//				{
	//					AvaliableRooms.Add(k);
	//				}
	//			}

	//			if (RandomRoom == -1)
	//			{
	//				if (AvaliableRooms.Num() > 0)
	//				{
	//					RandomRoom = AvaliableRooms[FMath::RandRange(0, AvaliableRooms.Num() - 1)];
	//				}
	//				else
	//				{
	//					RandomRoom = 0;
	//				}
	//			}
	//			if (i + j * Size.X == StartPos)
	//			{
	//				RandomRoom = 2;
	//				StartPostion = FVector(i * Offset.X, j * Offset.Y, 100.f);
	//			}
	//			else if (i + j * Size.X == BossCell)
	//			{
	//				RandomRoom = 0;
	//			}
	//			else
	//			{
	//				RandomRoom = 1;
	//			}


	//			
	//		}
	//	}
	//}
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
	for (int i = 0; i < Size.Y; ++i)
	{
		int32 Compare = 0;
		for (int j = 0; j < Size.X; j++)
		{
			FCell CurrentCell = Board[i * Size.Y + j]; //0 1 2 3 4 ¼ø
			if (CurrentCell.Visited)
			{
				Compare++;
			}
		}
		MaxWidth = FMath::Max(Compare, MaxWidth);
	}


	for (int i = 0; i < Size.X; ++i)
	{
		int32 Compare = 0;
		for (int j = 0; j < Size.Y; ++j)
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
