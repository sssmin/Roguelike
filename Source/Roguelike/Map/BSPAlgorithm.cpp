// Fill out your copyright notice in the Description page of Project Settings.
#include "BSPAlgorithm.h"
#include "Components/InstancedStaticMeshComponent.h"

ABSPAlgorithm::ABSPAlgorithm()
{
	PrimaryActorTick.bCanEverTick = false;
	MapSize = FIntVector2(100, 100);
	MinDivideRate = 0.3f;
	MaxDivideRate = 0.7f;
	MaxDepth = 3;

	ISMFloor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISMFloor"));
	RootComponent = ISMFloor;

	ISMWall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISMWall"));
	ISMWall->SetupAttachment(RootComponent);

	ISMLoad = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISMLoad"));
	ISMLoad->SetupAttachment(RootComponent);

	MinXY = FIntVector2(50'000, 50'000);
	MaxXY = FIntVector2(0, 0);
}

void ABSPAlgorithm::BeginPlay()
{
	Super::BeginPlay();
}

void ABSPAlgorithm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABSPAlgorithm::MakeMap()
{
	FillBackground();

	Root = MakeShared<FRLNode>(MakeShared<FRect>(0, 0, MapSize.X, MapSize.Y));
	Divide(Root, 0);
	GenerateRoom(Root, 0);

	GenerateLoad(Root, 0);
	FillWall();

	OnCreateMapComplete.ExecuteIfBound();
}

FVector ABSPAlgorithm::TestPlayerLocBossLoc()
{
	return FVector((MinXY.X - MapSize.X / 2) * 200, (MinXY.Y - MapSize.Y / 2) * 200, 0);
	//FVector((MaxXY.X - MapSize.X / 2) * 400, (MaxXY.Y - MapSize.Y / 2) * 400, 0);
}

void ABSPAlgorithm::Divide(TSharedPtr<FRLNode> Tree, int32 N)
{
	if (N == MaxDepth) return;

	int32 MaxLength = FMath::Max(Tree->NodeRect->Width, Tree->NodeRect->Height);
	int32 Split = FMath::RoundToInt32(FMath::RandRange(MaxLength * MinDivideRate, MaxLength * MaxDivideRate));

	if (Tree->NodeRect->Width >= Tree->NodeRect->Height)
	{
		Tree->LeftNode = MakeShared<FRLNode>(MakeShared<FRect>(Tree->NodeRect->X, Tree->NodeRect->Y, Split, Tree->NodeRect->Height));
		Tree->RightNode = MakeShared<FRLNode>(MakeShared<FRect>(Tree->NodeRect->X + Split, Tree->NodeRect->Y, Tree->NodeRect->Width - Split, Tree->NodeRect->Height));
	}
	else
	{
		Tree->LeftNode = MakeShared<FRLNode>(MakeShared<FRect>(Tree->NodeRect->X, Tree->NodeRect->Y, Tree->NodeRect->Width, Split));
		Tree->RightNode = MakeShared<FRLNode>(MakeShared<FRect>(Tree->NodeRect->X, Tree->NodeRect->Y + Split, Tree->NodeRect->Width, Tree->NodeRect->Height - Split));
	}
	Tree->LeftNode->ParNode = Tree;
	Tree->RightNode->ParNode = Tree;

	Divide(Tree->LeftNode, N + 1);
	Divide(Tree->RightNode, N + 1);

}

TSharedPtr<FRect> ABSPAlgorithm::GenerateRoom(TSharedPtr<FRLNode> Tree, int32 N)
{
	TSharedPtr<FRect> Rect;
	if (N == MaxDepth)
	{
		Rect = Tree->NodeRect;
		int32 Width = FMath::RandRange(Rect->Width / 2, Rect->Width - 1);
		int32 Height = FMath::RandRange(Rect->Height / 2, Rect->Height - 1);

		int32 X = Rect->X + FMath::RandRange(1, Rect->Width - Width);
		int32 Y = Rect->Y + FMath::RandRange(1, Rect->Height - Height);
		if (X < MinXY.X && Y < MinXY.Y)
		{
			MinXY.X = X;
			MinXY.Y = Y;
		}
		if (MaxXY.X < X && MaxXY.Y < Y)
		{
			MaxXY.X = X;
			MaxXY.Y = Y;
		}


		Rect = MakeShared<FRect>(X, Y, Width - 1, Height - 1);
		FillFloor(Rect);
	}
	else
	{
		Tree->LeftNode->RoomRect = GenerateRoom(Tree->LeftNode, N + 1);
		Tree->RightNode->RoomRect = GenerateRoom(Tree->RightNode, N + 1);
		Rect = Tree->LeftNode->RoomRect;
	}
	return Rect;
}

void ABSPAlgorithm::GenerateLoad(TSharedPtr<FRLNode> Tree, int32 N)
{
	if (N == MaxDepth) return;
	FIntVector2 LeftNodeCenter = Tree->LeftNode->GetCenter();
	FIntVector2 RightNodeCenter = Tree->RightNode->GetCenter();

	FTransform SpawnTransform;
	FRotator SpawnRot(0);
	FVector Scale(1.f, 1.f, 1.f);
	FVector Loc;

	int XMin = FMath::Min(LeftNodeCenter.X, RightNodeCenter.X);
	int XMax = FMath::Max(LeftNodeCenter.X, RightNodeCenter.X);
	int YMin = FMath::Min(LeftNodeCenter.Y, RightNodeCenter.Y);
	int YMax = FMath::Max(LeftNodeCenter.Y, RightNodeCenter.Y);

	UE_LOG(LogTemp, Warning, TEXT("xmin : %d, xmax : %d, ymin : %d, ymax : %d"), XMin, XMax, YMin, YMax);

	for (int i = XMin; i <= XMax; ++i)
	{
		if (ISMLoad)
		{
			Loc = FVector((i - MapSize.X / 2) * 200, (LeftNodeCenter.Y - MapSize.Y / 2) * 200, 0);

			RoomLoc.Add(Loc, true);
			if (BackgroundLoc.Find(Loc))
				BackgroundLoc.Remove(Loc);
			SpawnTransform = FTransform(SpawnRot, Loc, Scale);
			ISMLoad->AddInstance(SpawnTransform, true);
		}
	}

	for (int i = YMin; i <= YMax; ++i)
	{
		if (ISMLoad)
		{
			Loc = FVector((RightNodeCenter.X - MapSize.X / 2) * 200, (i - MapSize.Y / 2) * 200, 0);

			RoomLoc.Add(Loc, true);
			if (BackgroundLoc.Find(Loc))
				BackgroundLoc.Remove(Loc);
			SpawnTransform = FTransform(SpawnRot, Loc, Scale);
			ISMLoad->AddInstance(SpawnTransform, true);
		}
	}

	GenerateLoad(Tree->LeftNode, N + 1);
	GenerateLoad(Tree->RightNode, N + 1);
}

void ABSPAlgorithm::FillBackground()
{
	FTransform SpawnTransform;
	FRotator SpawnRot(0);
	FVector Scale(1.f, 1.f, 1.f);
	FVector Loc;
	for (int i = -10; i < MapSize.X + 10; ++i)
	{
		for (int j = -10; j < MapSize.Y + 10; ++j)
		{
			if (ISMFloor)
			{
				Loc = FVector((i - MapSize.X / 2) * 200, (j - MapSize.Y / 2) * 200, 0);
				BackgroundLoc.Add(Loc, true);
				SpawnTransform = FTransform(SpawnRot, Loc, Scale);
			}
		}
	}
}

void ABSPAlgorithm::FillWall() //바닥과 바깥이 만나는 부분
{
	FTransform SpawnTransform;
	FRotator SpawnRot(0);
	FVector Scale(4.2f, 4.2f, 20.f);
	FVector Loc;
	for (int i = 0; i < MapSize.X; ++i)
	{
		for (int j = 0; j < MapSize.Y; ++j)
		{
			if (BackgroundLoc.Find(FVector((i - MapSize.X / 2) * 200, (j - MapSize.Y / 2) * 200, 0)))
			{
				//바깥 타일일 경우
				for (int x = -1; x <= 1; ++x)
				{
					for (int y = -1; y <= 1; ++y)
					{
						if (x == 0 && y == 0) continue;//바깥 타일 기준 8방향 탐색해서 roomtile이 있다면 wall로
						if (RoomLoc.Find(FVector((i - MapSize.X / 2 + x) * 200, (j - MapSize.Y / 2 + y) * 200, 0)))
						{
							Loc = FVector((i - MapSize.X / 2) * 200, (j - MapSize.Y / 2) * 200, 0);
							SpawnTransform = FTransform(SpawnRot, Loc, Scale);
							ISMWall->AddInstance(SpawnTransform, true);
							break;
						}
					}
				}
			}
		}
	}
}

void ABSPAlgorithm::FillFloor(TSharedPtr<FRect> Rect) //하나의 방 바닥을 채운다.
{
	FTransform SpawnTransform;
	FRotator SpawnRot(0);
	FVector Scale(1.f, 1.f, 1.f);
	FVector Loc;

	int32 Check = 0;
	
	for (int i = Rect->X; i < Rect->X + Rect->Width; ++i)
	{
		for (int j = Rect->Y; j < Rect->Y + Rect->Height; ++j)
		{
			if (ISMFloor)
			{
				Loc = FVector((i - MapSize.X / 2) * 200, (j - MapSize.Y / 2) * 200, 0);
				if (Check == 0)
				{
					RoomInfos.Add(FRoomInfo(Rect, Loc));
				}
				RoomLoc.Add(Loc, true);
				if (BackgroundLoc.Find(Loc))
					BackgroundLoc.Remove(Loc);
				SpawnTransform = FTransform(SpawnRot, Loc, Scale);
				ISMFloor->AddInstance(SpawnTransform, true);
				Check = 1;
			}
		}
	}
}