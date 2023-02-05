// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameInstance.h"
#include "Roguelike/Map/DFSAgrt.h"
#include "Roguelike/PlayerController/RLPlayerController.h"

URLGameInstance::URLGameInstance()
{
	DFS = MakeShared<DFSAgrt>();
	
	StartCell = 0;
	PlayerCurrentCell = 0;
	StageLevel = 1;
	BossCell = 0;
	BossPrevCell = 0;

	ClearCount = 0;
	bIsDiscoverdBoss = false;

}

void URLGameInstance::Init()
{
	Super::Init();
}

void URLGameInstance::Initialize()
{
	GenerateMap();
	HealthManage = FHealthManage();
	CombatManage = FCombatManage();
	State = 0;
	PlayerSpawnLoc = FVector(0.f, 0.f, 0.f);
}

void URLGameInstance::GenerateMap()
{
	if (DFS.IsValid())
	{
		MapSize = FVector2Int(4 + StageLevel, 4 + StageLevel);
		DFS->StartAlgorithm(MapSize);
		StartPos = DFS->GetStartPostion();
		Board = DFS->GetBoard();
		StartCell = DFS->GetStartCell();
		PlayerCurrentCell = StartCell;
		BossPrevCell = DFS->GetBossPrevCell();
		BossCell = DFS->GetBossCell();
		TotalCellNum = DFS->GetTotalCellNum();
	}
}

void URLGameInstance::TestPrintMap()
{
	for (int32 i = 0; i < Board.Num(); ++i)
	{
		for (int32 j = 0; j < Board[i].Status.Num(); ++j)
		{
			UE_LOG(LogTemp, Warning, TEXT("Board %d : Status - %d"), i, Board[i].Status[j]);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CurrentCell : %d"), PlayerCurrentCell);
	UE_LOG(LogTemp, Warning, TEXT("TotalCellNum : %d"), TotalCellNum);
	UE_LOG(LogTemp, Warning, TEXT("BossCell : %d"), BossCell);
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

void URLGameInstance::RequestMove(int32 Dir, const FVector& OtherSide) //0,1,2,3중 하나로 넘어옴
{
	int32 NextCell = CalcNextCell(Dir);
	if (NextCell == BossCell) //가려는 셀이 보스셀일 때
	{
		if (TotalCellNum - 1 != ClearCount) //반대편 셀을 모두 클리어하지 않았으면
		{
			if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
			{
				Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->ShowNoticeWidget();
			}
			return;
		}
	}
	else if (NextCell == BossPrevCell)
	{
		Board[BossCell].CellState = ECellState::DISCOVEREDBOSS;
	}

	Board[PlayerCurrentCell].CellState = ECellState::CLEAR;
	PlayerCurrentCell = NextCell;
	Board[PlayerCurrentCell].CellState = ECellState::INPLAYER;

	PlayerSpawnLoc = OtherSide;
	OnMoveMap.ExecuteIfBound(); //매니저 받아오기
	if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RemoveMinimapWidget();
		if (PlayerCurrentCell == StartCell)
		{
			GetFirstLocalPlayerController(GetWorld())->ClientTravel("/Game/Maps/StartMap", ETravelType::TRAVEL_Relative, true);
		}
		else
		{
			GetFirstLocalPlayerController(GetWorld())->ClientTravel("/Game/Maps/GameMap2", ETravelType::TRAVEL_Relative, true);
		}
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

void URLGameInstance::ClearStage()
{
	StageLevel++;
	ClearCount = 0;
	bIsDiscoverdBoss = false;
	PlayerSpawnLoc = FVector(0.f, 0.f, 0.f);
	GenerateMap();
	
	OnMoveMap.ExecuteIfBound(); 

	if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RemoveMinimapWidget();
		GetFirstLocalPlayerController(GetWorld())->ClientTravel("/Game/Maps/StartMap", ETravelType::TRAVEL_Relative, true);
	}
}