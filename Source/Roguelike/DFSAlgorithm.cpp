// Fill out your copyright notice in the Description page of Project Settings.


#include "DFSAlgorithm.h"
#include "RoomActor.h"

ADFSAlgorithm::ADFSAlgorithm()
{
	PrimaryActorTick.bCanEverTick = false;
	StartPos = 0;
	Parts = 1;
	CreatedPart = 0;
	BossCell = 0;
	StartPostion = FVector::ZeroVector;
}

void ADFSAlgorithm::BeginPlay()
{
	Super::BeginPlay();
	StartAlgorithm();
}

FVector ADFSAlgorithm::StartAlgorithm()
{
	Init();
	MazeGenerator();
	GenerateDungeon();

	return StartPostion;
}

void ADFSAlgorithm::Init()
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

void ADFSAlgorithm::MazeGenerator() //�ϳ��� ������ �� ����
{
	int32 CurrentCell = StartPos;
		
	TArray<int> Path;
	TArray<int32> SideCells;

	int32 CreatedRoomCount = 0;

	while (true)
	{
		SideCells.Empty();

		Board[CurrentCell].Visited = true;

		if (CreatedRoomCount == RoomCount) break;
		CreatedRoomCount++;

		if (CurrentCell == Board.Num() - 1)
			break;

		SideCells = CheckSideCell(CurrentCell);

		if (SideCells.Num() != 0)
		{
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
	BossCell = CurrentCell;
	
	if (++CreatedPart != Parts)
		MazeGenerator();
}

void ADFSAlgorithm::GenerateDungeon()
{
	for (int i = 0; i < Size.X; ++i)
	{
		for (int j = 0; j < Size.Y; ++j)
		{
			FCell CurrentCell = Board[i + j * Size.X];
			if (CurrentCell.Visited)
			{
				int32 RandomRoom = -1;
				TArray<int32> AvaliableRooms;
				for (int k = 0; k < Rooms.Num(); ++k)
				{
					int32 P = Rooms[k].ProbilityOfSpawning(i, j);

					if (P == 2)
					{
						RandomRoom = k;
						break;
					}
					else if (P == 1)
					{
						AvaliableRooms.Add(k);
					}
				}

				if (RandomRoom == -1)
				{
					if (AvaliableRooms.Num() > 0)
					{
						RandomRoom = AvaliableRooms[FMath::RandRange(0, AvaliableRooms.Num() - 1)];
					}
					else
					{
						RandomRoom = 0;
					}
				}
				if (i + j * Size.X == StartPos)
				{
					RandomRoom = 2;
					StartPostion = FVector(i * Offset.X, j * Offset.Y, 100.f);
				}
				else if (i + j * Size.X == BossCell)
				{
					RandomRoom = 0;
				}
				else
				{
					RandomRoom = 1;
				}
					

				//�� ����
				FTransform SpawnTransfrom = FTransform(FRotator::ZeroRotator, FVector(i * Offset.X, j * Offset.Y, 0), FVector(1.f, 1.f, 1.f));
				ARoomActor* NewRoom = GetWorld()->SpawnActor<ARoomActor>(Rooms[RandomRoom].Room, SpawnTransfrom);
				//�� �շ��ִ� ���� ������Ʈ
				if(NewRoom)
					NewRoom->UpdateRoom(CurrentCell.Status);
			}
		}
	}
}

TArray<int32> ADFSAlgorithm::CheckSideCell(int32 Cell)
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