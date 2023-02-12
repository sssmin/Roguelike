// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameInstance.h"
#include "Roguelike/Map/DFSAgrt.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "RLGameStateBase.h"
#include "Kismet/GameplayStatics.h"

URLGameInstance::URLGameInstance()
{
	DFS = MakeShared<DFSAgrt>();
	
	Initialize();
}

void URLGameInstance::Init()
{
	Super::Init();
}

void URLGameInstance::NewGame()
{
	Initialize();
	GenerateMap();
}

void URLGameInstance::Initialize()
{
	Board.Empty();
	StartPos = FVector::ZeroVector;
	StartCell = 0;
	PlayerCurrentCell = 0;
	StageLevel = 2;
	BossCell = 0;
	BossPrevCell = 0;

	TotalCellNum = 0;

	HealthManager = FHealthManager();
	CombatManager = FCombatManager();

	Buff = 0;
	RLGameState = Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this));
}

void URLGameInstance::GenerateMap()
{
	if (DFS.IsValid())
	{
		MapSize = FVector2Int(4 + StageLevel, 4 + StageLevel);
		DFS->StartAlgorithm(MapSize);
		Board = DFS->GetBoard();
		StartPos = DFS->GetStartPostion();
		StartCell = DFS->GetStartCell();
		PlayerCurrentCell = StartCell;
		BossCell = DFS->GetBossCell();
		BossPrevCell = DFS->GetBossPrevCell();
		TotalCellNum = DFS->GetTotalCellNum();
	}
	ClearCount = 0;
	bIsDiscoverdBoss = false;
}

void URLGameInstance::TestPrintMap()
{
	/*for (int32 i = 0; i < Board.Num(); ++i)
	{
		for (int32 j = 0; j < Board[i].Status.Num(); ++j)
		{
			UE_LOG(LogTemp, Warning, TEXT("Board %d : Status - %d"), i, Board[i].Status[j]);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CurrentCell : %d"), PlayerCurrentCell);
	UE_LOG(LogTemp, Warning, TEXT("TotalCellNum : %d"), TotalCellNum);
	UE_LOG(LogTemp, Warning, TEXT("BossCell : %d"), BossCell);*/

	ClearStage();
}

void URLGameInstance::RequestInfo()
{
	if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->SetMapInfo(MapSize, Board, PlayerCurrentCell);
	}
}

TArray<int32> URLGameInstance::GetConnectedDir()
{
	TArray<int32> Ret;
	if (PlayerCurrentCell - MapSize.Y >= 0) //위에 셀이 있다.
	{
		if (Board[PlayerCurrentCell].Status[0] && Board[PlayerCurrentCell - MapSize.Y].Status[1])
		{
			Ret.Add(0);
		}
	}
	if (PlayerCurrentCell + MapSize.Y < Board.Num()) //아래 셀이 있다.
	{
		if (Board[PlayerCurrentCell].Status[1] && Board[PlayerCurrentCell + MapSize.Y].Status[0])
		{
			Ret.Add(1);
		}
	}
	if ((PlayerCurrentCell + 1) % MapSize.Y != 0) //오른쪽 셀이 있다
	{
		if (Board[PlayerCurrentCell].Status[2] && Board[PlayerCurrentCell + 1].Status[3])
		{
			Ret.Add(2);
		}
	}
	if (PlayerCurrentCell % MapSize.X != 0) //왼쪽 셀이 있다.
	{
		if (Board[PlayerCurrentCell].Status[3] && Board[PlayerCurrentCell - 1].Status[2])
		{
			Ret.Add(3);
		}
	}

	return Ret;
}

void URLGameInstance::RequestMove(int32 Dir) //0,1,2,3중 하나로 넘어옴
{
	int32 NextCell = CalcNextCell(Dir);
	if (NextCell == BossCell) //가려는 셀이 보스셀일 때
	{
		if (TotalCellNum - 1 != ClearCount) //반대편 셀을 모두 클리어하지 않았으면
		{
			if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
			{
				Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->ShowNoticeWidget(TEXT("다른 방을 클리어해야 해요."));
			}
			return;
		}
	}
	else if (NextCell == BossPrevCell)
	{
		Board[BossCell].CellState = ECellState::DISCOVERED_BOSS;
	}

	Board[PlayerCurrentCell].CellState = ECellState::CLEAR;
	PlayerCurrentCell = NextCell;
	Board[PlayerCurrentCell].CellState = ECellState::IN_PLAYER;

	if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RemoveMinimapWidget();
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->SetMapInfo(MapSize, Board, PlayerCurrentCell);
	}
	RLGameState = RLGameState == nullptr ? Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this)) : RLGameState;
	if (RLGameState)
	{
		RLGameState->ReconstructCuzMove(Dir, StageLevel, Board[PlayerCurrentCell]);
	}
}

int32 URLGameInstance::CalcNextCell(int32 Dir)
{
	switch (Dir)
	{
		case 0:
			return PlayerCurrentCell - MapSize.Y;
		case 1:
			return PlayerCurrentCell + MapSize.Y;
		case 2:
			return PlayerCurrentCell + 1;
		case 3:
			return PlayerCurrentCell - 1;
	}
	return 0;
}

void URLGameInstance::ClearThisCell()
{
	Board[PlayerCurrentCell].IsCleared = true;
	ClearCount++;
}

void URLGameInstance::AteHealThisCell()
{
	Board[PlayerCurrentCell].IsCleared = true;
}

void URLGameInstance::RequestMoveNextStage()
{
	StageLevel++;
	GenerateMap();
	
	//OnMoveMap.ExecuteIfBound(); 

	
}

void URLGameInstance::MoveNextStage(const FString& MapName)
{
	/*if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		GetFirstLocalPlayerController(GetWorld())->ClientTravel(MapName, ETravelType::TRAVEL_Relative, true);
	}*/
}

void URLGameInstance::ClearStage()
{
	if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RemoveMinimapWidget();
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->ShowSelectItemWidget();
	}
}