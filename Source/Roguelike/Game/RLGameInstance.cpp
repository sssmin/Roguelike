// Fill out your copyright notice in the Description page of Project Settings.
#include "RLGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "Roguelike/Map/DFSAgrt.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "RLListenerManager.h"
#include "RLGameStateBase.h"


URLGameInstance::URLGameInstance()
{
	
}

void URLGameInstance::Init()
{
	Super::Init();

	DFS = MakeShared<DFSAgrt>();
	ListenerManager = NewObject<URLListenerManager>();
	Initialize();
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
	StageLevel = 1;
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
		MapSize = FVector2Int(2 + StageLevel, 2 + StageLevel);
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
	bIsEarlyDiscoveredBoss = false;
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
	UE_LOG(LogTemp, Warning, TEXT("StartCell : %d"), StartCell);
	UE_LOG(LogTemp, Warning, TEXT("BossCell : %d"), BossCell);
	UE_LOG(LogTemp, Warning, TEXT("BossPrevCell : %d"), BossPrevCell);
	UE_LOG(LogTemp, Warning, TEXT("TotalCellNum : %d"), TotalCellNum);
	UE_LOG(LogTemp, Warning, TEXT("ClearCount : %d"), ClearCount);
	UE_LOG(LogTemp, Warning, TEXT("TotalCellNum - 1 == ClearCount -> Can Move Boss"));


	//RequestMoveNextStage();
	//ClearStage();
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
	if (NextCell == BossCell)
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
		if (TotalCellNum - 2 > ClearCount) //조기 발견
		{
			bIsEarlyDiscoveredBoss = true;
		}
		Board[BossCell].CellState = ECellState::DISCOVERED_BOSS;
	}

	MoveProcess(NextCell, Dir);
	if (PlayerCurrentCell != BossCell)
	{
		CheckEarlyDiscoveredBossCell();
	}
}

void URLGameInstance::RequestMovePrevBossCell()
{
	MoveProcess(BossPrevCell, -1);
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
	CheckEarlyDiscoveredBossCell();
}

void URLGameInstance::CheckEarlyDiscoveredBossCell()
{
	if ((TotalCellNum - 1 == ClearCount) && bIsEarlyDiscoveredBoss)
	{
		RLGameState = RLGameState ? RLGameState : Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this));
		if (RLGameState)
		{
			RLGameState->SpawnPrevBossPortal();
		}
	}
}

void URLGameInstance::AteHealThisCell()
{
	Board[PlayerCurrentCell].IsCleared = true;
}

void URLGameInstance::RequestMoveNextStage()
{
	StageLevel++;
	GenerateMap();
	
	InitOnceItemDelegate.ExecuteIfBound();
	MoveProcess(StartCell, -1);
}

void URLGameInstance::ReadyToRecall()
{
	if (Board[PlayerCurrentCell].IsCleared)
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RecallStart();
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RecallCompleteSuccessfully.BindUObject(this, &ThisClass::Recall);
	}
}

void URLGameInstance::Recall()
{
	MoveProcess(StartCell, -1);
}

void URLGameInstance::MoveProcess(int32 TargetCell, int32 Dir)
{
	Board[PlayerCurrentCell].CellState = ECellState::CLEAR;
	if ((Board[PlayerCurrentCell].CellType == ECellType::BONUS) && !Board[PlayerCurrentCell].IsCleared)
	{
		Board[PlayerCurrentCell].CellState = ECellState::BONUS;
	}
	PlayerCurrentCell = TargetCell;
	Board[PlayerCurrentCell].CellState = ECellState::IN_PLAYER;

	if (Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld())))
	{
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->RemoveMinimapWidget();
		Cast<ARLPlayerController>(GetFirstLocalPlayerController(GetWorld()))->SetMapInfo(MapSize, Board, PlayerCurrentCell);
	}
	RLGameState = RLGameState ? RLGameState : Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this));
	if (RLGameState)
	{
		RLGameState->ReconstructCuzMove(Dir, StageLevel, Board[PlayerCurrentCell]);
	}
}

URLListenerManager* URLGameInstance::GetListenerManager()
{
	return ListenerManager;
}