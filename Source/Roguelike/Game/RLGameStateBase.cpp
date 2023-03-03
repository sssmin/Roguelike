// Fill out your copyright notice in the Description page of Project Settings.
#include "RLGameStateBase.h"
#include "Kismet/GameplayStatics.h"

#include "RLGameInstance.h"
#include "RLGameModeBase.h"
#include "Roguelike/PlayerController/RLPlayerController.h"


ARLGameStateBase::ARLGameStateBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ObjectiveNum = 0;
	CurrentNum = 0;
	StageLevel = 0;
}

void ARLGameStateBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARLGameStateBase::Init()
{
	RLGameMode = Cast<ARLGameModeBase>(UGameplayStatics::GetGameMode(this));
	RLGameInst = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	RLPlayerController = Cast<ARLPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	check(RLGameMode);
	check(RLGameInst);
	
	StageLevel = RLGameInst->GetStageLevel();
	CellInfo = RLGameInst->GetCellInfo();
	RLGameMode->RequestSpawnCell(CellInfo.CellClass, CellInfo.TempWall);
	SetObjective();
	
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
	check(RLGameMode);
	RLGameMode->RequestSpawnCell(CellInfo.CellClass, CellInfo.TempWall, Dir);
}


void ARLGameStateBase::SetObjective()
{
	CurrentNum = 0;

	check(RLGameInst);
	check(RLGameMode);
	check(RLPlayerController);
	
	if (CellInfo.IsCleared)
	{
		CreateSidePortal();
	}
	else
	{
		switch (CellInfo.CellType)
		{
		case ECellType::MOBS:
			if (FMath::RandBool())
			{
				ObjectiveNum = 4;
			}
			else
			{
				ObjectiveNum = 6;
			}
			//ObjectiveNum = 1;//TEST
			RLGameMode->RequestSpawnMob(StageLevel, ObjectiveNum++); //기본 터렛 1 ++
			break;
		case ECellType::BOSS:
			ObjectiveNum = 1;
			RLGameMode->RequestSpawnBoss(StageLevel);
			break;
		case ECellType::BONUS:
			RLGameMode->RequestSpawnHealItem();
			if (!CellInfo.SelectedBonusItem)
			{
				RLGameInst->SetSelectedBonusItem(true);
				RLPlayerController->ShowSelectItemWidget();
			}
			CreateSidePortal();
			break;
		}
	}
}

void ARLGameStateBase::KillBoss()
{
	CreateCenterPortal();
	check(RLPlayerController);
	RLPlayerController->ShowSelectItemWidget();
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
	check(RLGameInst);
	RLGameInst->ClearThisCell();
}

void ARLGameStateBase::AteHealThisCell()
{
	check(RLGameInst);
	RLGameInst->AteHealThisCell();
}

void ARLGameStateBase::CreateSidePortal()
{
	check(RLGameMode);
	RLGameMode->CreateSidePortal();
}

void ARLGameStateBase::CreateCenterPortal()
{
	check(RLGameMode);
	RLGameMode->CreateCenterPortal();
}

void ARLGameStateBase::SpawnPrevBossPortal()
{
	check(RLGameMode);
	RLGameMode->CreatePrevBossPortal();
}