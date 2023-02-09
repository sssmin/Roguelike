// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameStateBase.h"
#include "RLGameInstance.h"
#include "RLGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ARLGameStateBase::ARLGameStateBase()
{
	ObjectiveNum = 0;
	CurrentNum = 0;
	StageLevel = 0;
}

void ARLGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	RLGameMode = Cast<ARLGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GetWorld())
	{
		URLGameInstance* RLGameInstance = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (RLGameInstance)
		{
			StageLevel = RLGameInstance->GetStageLevel();
			CellInfo = RLGameInstance->GetCellInfo();
			RLGameMode->SpawnCell(CellInfo.CellClass, CellInfo.TempWall);
			SetObjective();
		}
	}
	
}

void ARLGameStateBase::ReconstructCuzMove(int32 Dir, int32 Level, const FCell& Info)
{
	StageLevel = Level;
	CellInfo = Info;
	SpawnCell(Dir);
	SetObjective();
}

void ARLGameStateBase::SpawnCell(int32 Dir)
{
	if (RLGameMode)
	{
		RLGameMode->SpawnCell(CellInfo.CellClass, CellInfo.TempWall, Dir);
	}
}


void ARLGameStateBase::SetObjective()
{
	CurrentNum = 0;
	if (CellInfo.IsCleared)
	{
		CreateSidePortal();
	}
	else
	{
		if (RLGameMode)
		{
			switch (CellInfo.CellType)
			{
			case ECellType::MOBS:
				ObjectiveNum = FMath::RandRange(4, 6);
				RLGameMode->SpawnMob(StageLevel, ObjectiveNum);
				break;
			case ECellType::BOSS:
				ObjectiveNum = 1;
				RLGameMode->SpawnBoss(StageLevel);
				break;
			case ECellType::BONUS:
				RLGameMode->SpawnHealItem();
				CreateSidePortal();
				break;
			}
		}
	}
}

void ARLGameStateBase::KillBoss()
{
	CreateCenterPortal();
	URLGameInstance* RLGameInstance = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (RLGameInstance)
	{
		RLGameInstance->ClearStage();
	}
}

void ARLGameStateBase::KillScored()
{
	if (++CurrentNum == ObjectiveNum)
	{
		ClearThisCell();
		CreateSidePortal();
	}
}

void ARLGameStateBase::ClearThisCell() //어떠한 조건으로 클리어 했을 때
{
	if (GetWorld())
	{
		URLGameInstance* RLGameInstance = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (RLGameInstance)
		{
			RLGameInstance->ClearThisCell();
		}
	}
}

void ARLGameStateBase::AteHealThisCell()
{
	if (GetWorld())
	{
		URLGameInstance* RLGameInstance = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (RLGameInstance)
		{
			RLGameInstance->AteHealThisCell();
		}
	}
}

void ARLGameStateBase::TestKillScored()
{
	if (++CurrentNum == ObjectiveNum)
	{
		ClearThisCell();
	}
}

void ARLGameStateBase::CreateSidePortal()
{
	if (RLGameMode)
	{
		RLGameMode->CreateSidePortal();
	}
}

void ARLGameStateBase::CreateCenterPortal()
{
	if (RLGameMode)
	{
		RLGameMode->CreateCenterPortal();
	}
}