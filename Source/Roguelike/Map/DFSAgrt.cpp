// Fill out your copyright notice in the Description page of Project Settings.

#include "DFSAgrt.h"

DFSAgrt::DFSAgrt()
{
	StartPos = 0;
	Parts = 1;
	CreatedPart = 0;
	BossCell = 0;
	BossPrevCell = 0;
	StartPostion = FVector::ZeroVector;
	TotalRoomNum = 0;
}

DFSAgrt::~DFSAgrt()
{

}

void DFSAgrt::StartAlgorithm(FVector2Int MapSize)
{
	Size = MapSize;
	Init();
	MazeGenerator();
	GenerateDungeon();
}

void DFSAgrt::Init()
{
	for (int i = 0; i < Size.X * Size.Y; ++i)
	{
		Board.Add(FCell());
	}

	RoomCount = (Size.X + Size.Y) / 2;
	Parts = FMath::RandRange(1, 3);
	if (Parts == 1)
		RoomCount += RoomCount / 2;

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

	int32 CreatedRoomCount = 0;
	int32 BeforeCell = 0;

	while (true)
	{
		SideCells.Empty();

		Board[CurrentCell].Visited = true;
		Board[CurrentCell].CellState = ECellState::NORMAL;
		Board[CurrentCell].CellType = ECellType::MOBS;
		
		if (CreatedRoomCount == RoomCount) break;

		if (CurrentCell == Board.Num()) break;

		SideCells = CheckSideCell(CurrentCell);

		if (SideCells.Num() != 0)
		{
			BeforeCell = CurrentCell;
			CreatedRoomCount++;
			TotalRoomNum++;

			int32 NewCell = SideCells[FMath::RandRange(0, SideCells.Num() - 1)];

			if (NewCell > CurrentCell) //이웃 셀이 현재 셀보다 크면? 아래 아니면 오른쪽 셀이다.
			{
				if (NewCell - 1 == CurrentCell) //이웃 셀은 오른쪽 셀
				{
					Board[CurrentCell].Status[2] = true;   //0 위 1 아래 2 오른 3 왼
					CurrentCell = NewCell;
					Board[CurrentCell].Status[3] = true;
				}
				else //이웃 셀은 아래 셀
				{
					Board[CurrentCell].Status[1] = true;
					CurrentCell = NewCell;
					Board[CurrentCell].Status[0] = true;
				}
			}
			else //이웃 셀은 위 아니면 왼쪽 셀이다.
			{
				if (NewCell + 1 == CurrentCell) //이웃 셀은 왼쪽 셀
				{
					Board[CurrentCell].Status[3] = true;
					CurrentCell = NewCell;
					Board[CurrentCell].Status[2] = true;
				}
				else //이웃 셀은 위 셀
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

	if (Cell - Size.Y >= 0 && !Board[Cell - Size.Y].Visited) //위의 셀이 있고, 위의 셀을 방문하지 않았으면
	{
		SideCells.Add(Cell - Size.Y);
	}
	if (Cell + Size.Y < Board.Num() && !Board[Cell + Size.Y].Visited) //아래 셀이 있고, 아래 셀을 방문하지 않았으면
	{
		SideCells.Add(Cell + Size.Y);
	}
	if ((Cell + 1) % Size.Y != 0 && !Board[Cell + 1].Visited) //오른쪽 셀이 있고, 오른쪽 셀을 방문하지 않았으면
	{
		SideCells.Add(Cell + 1); //자기자신이 오른쪽 끝에 해당하면 오른쪽이 없는 것. if문 첫번째가 그걸 판별
	}
	if (Cell % Size.X != 0 && !Board[Cell - 1].Visited) //왼쪽 셀이 있고, 왼쪽 셀을 방문하지 않았으면
	{
		SideCells.Add(Cell - 1); //자기자신이 왼쪽 끝에 해당하면 왼쪽이 없는 것. if문 첫번째가 그걸 판별
	}
	//0, 1, 2, 3, 4
	//5, 6, 7, 8, 9... xsize 5, ysize 5일 때 4의 오른쪽은 없고, 5의 왼쪽은 없다.

	//셀을 배열로 관리하기 때문에 위 같이 계산이 된다. 또한 방 크기가 같기 때문에
	//0을 스타트 포인트로 한다면 0, 0 이고 1은 방 크기 offset만큼 오른쪽으로 이동한 것 = 방마다 좌표 계산이 쉽다.

	return SideCells;
}

void DFSAgrt::GetMaxWidgetHeight(OUT int32 MaxWidth, OUT int32 MaxHeight)
{
	for (int i = 0; i < Size.Y; ++i)
	{
		int32 Compare = 0;
		for (int j = 0; j < Size.X; j++)
		{
			FCell CurrentCell = Board[i * Size.Y + j]; //0 1 2 3 4 순
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
			FCell CurrentCell = Board[i + j * Size.X]; //0 5 10 15 순
			if (CurrentCell.Visited)
			{
				Compare++;
			}
		}
		MaxHeight = FMath::Max(Compare, MaxHeight);
	}
	
}
