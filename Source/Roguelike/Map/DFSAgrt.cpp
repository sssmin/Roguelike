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

			if (NewCell > CurrentCell) //�̿� ���� ���� ������ ũ��? �Ʒ� �ƴϸ� ������ ���̴�.
			{
				if (NewCell - 1 == CurrentCell) //�̿� ���� ������ ��
				{
					Board[CurrentCell].Status[2] = true;   //0 �� 1 �Ʒ� 2 ���� 3 ��
					CurrentCell = NewCell;
					Board[CurrentCell].Status[3] = true;
				}
				else //�̿� ���� �Ʒ� ��
				{
					Board[CurrentCell].Status[1] = true;
					CurrentCell = NewCell;
					Board[CurrentCell].Status[0] = true;
				}
			}
			else //�̿� ���� �� �ƴϸ� ���� ���̴�.
			{
				if (NewCell + 1 == CurrentCell) //�̿� ���� ���� ��
				{
					Board[CurrentCell].Status[3] = true;
					CurrentCell = NewCell;
					Board[CurrentCell].Status[2] = true;
				}
				else //�̿� ���� �� ��
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

	if (Cell - Size.Y >= 0 && !Board[Cell - Size.Y].Visited) //���� ���� �ְ�, ���� ���� �湮���� �ʾ�����
	{
		SideCells.Add(Cell - Size.Y);
	}
	if (Cell + Size.Y < Board.Num() && !Board[Cell + Size.Y].Visited) //�Ʒ� ���� �ְ�, �Ʒ� ���� �湮���� �ʾ�����
	{
		SideCells.Add(Cell + Size.Y);
	}
	if ((Cell + 1) % Size.Y != 0 && !Board[Cell + 1].Visited) //������ ���� �ְ�, ������ ���� �湮���� �ʾ�����
	{
		SideCells.Add(Cell + 1); //�ڱ��ڽ��� ������ ���� �ش��ϸ� �������� ���� ��. if�� ù��°�� �װ� �Ǻ�
	}
	if (Cell % Size.X != 0 && !Board[Cell - 1].Visited) //���� ���� �ְ�, ���� ���� �湮���� �ʾ�����
	{
		SideCells.Add(Cell - 1); //�ڱ��ڽ��� ���� ���� �ش��ϸ� ������ ���� ��. if�� ù��°�� �װ� �Ǻ�
	}
	//0, 1, 2, 3, 4
	//5, 6, 7, 8, 9... xsize 5, ysize 5�� �� 4�� �������� ����, 5�� ������ ����.

	//���� �迭�� �����ϱ� ������ �� ���� ����� �ȴ�. ���� �� ũ�Ⱑ ���� ������
	//0�� ��ŸƮ ����Ʈ�� �Ѵٸ� 0, 0 �̰� 1�� �� ũ�� offset��ŭ ���������� �̵��� �� = �渶�� ��ǥ ����� ����.

	return SideCells;
}

void DFSAgrt::GetMaxWidgetHeight(OUT int32 MaxWidth, OUT int32 MaxHeight)
{
	for (int i = 0; i < Size.Y; ++i)
	{
		int32 Compare = 0;
		for (int j = 0; j < Size.X; j++)
		{
			FCell CurrentCell = Board[i * Size.Y + j]; //0 1 2 3 4 ��
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
			FCell CurrentCell = Board[i + j * Size.X]; //0 5 10 15 ��
			if (CurrentCell.Visited)
			{
				Compare++;
			}
		}
		MaxHeight = FMath::Max(Compare, MaxHeight);
	}
	
}
