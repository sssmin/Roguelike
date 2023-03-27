// Fill out your copyright notice in the Description page of Project Settings.
#include "RLMainGameState.h"
#include "Kismet/GameplayStatics.h"

#include "RLGameInstance.h"
#include "RLMainGameMode.h"
#include "Roguelike/PlayerController/RLPlayerController.h"


ARLMainGameState::ARLMainGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	ObjectiveNum = 0;
	CurrentNum = 0;
	StageLevel = 0;
}

void ARLMainGameState::BeginPlay()
{
	Super::BeginPlay();

	RLGameMode = Cast<ARLMainGameMode>(UGameplayStatics::GetGameMode(this));

	RLPlayerController = Cast<ARLPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if(GetRLGameInst() && GetRLGameInst()->GetListenerManager())
	{
		GetRLGameInst()->GetListenerManager()->OnNewGameDelegate.AddUObject(this, &ThisClass::NewGame);
		GetRLGameInst()->GetListenerManager()->OnLoadGameDelegate.AddUObject(this, &ThisClass::LoadGame);
	}
}

void ARLMainGameState::NewGame()
{
	check(RLGameMode);
	check(GetRLGameInst());
	
	StageLevel = GetRLGameInst()->GetStageLevel();
	SetCellInfo(GetRLGameInst()->GetCellInfo());
	RLGameMode->RequestSpawnCell(GetCellInfo().CellClass, GetCellInfo().TempWall);
	SetObjective();
}

void ARLMainGameState::LoadGame()
{
	check(RLGameMode);
	check(GetRLGameInst());
	
	StageLevel = GetRLGameInst()->GetStageLevel();
	SetCellInfo(GetRLGameInst()->GetCellInfo());
	RLGameMode->RequestSpawnCell(GetCellInfo().CellClass, GetCellInfo().TempWall, static_cast<uint8>(EDir::Load));
	SetObjective();
}

void ARLMainGameState::ReconstructCell(uint8 Dir, int32 Level, const FCell& Info)
{
	check(RLGameMode);
	
	StageLevel = Level;
	SetCellInfo(Info);
	RLGameMode->RequestSpawnCell(GetCellInfo().CellClass, GetCellInfo().TempWall, Dir);
	SetObjective();
}

void ARLMainGameState::SetObjective()
{
	CurrentNum = 0;

	check(GetRLGameInst());
	check(RLGameMode);
	check(RLPlayerController);
	
	if (GetCellInfo().IsCleared)
	{
		CreateSidePortal();
	}
	else
	{
		switch (GetCellInfo().CellType)
		{
		case ECellType::Mobs:
			if (FMath::RandBool())
			{
				ObjectiveNum = 4;
			}
			else
			{
				ObjectiveNum = 6;
			}
			RLGameMode->RequestSpawnMob(StageLevel, ObjectiveNum++); //�⺻ �ͷ� 1 ++
			break;
		case ECellType::Boss:
			ObjectiveNum = 1;
			RLGameMode->RequestSpawnBoss(StageLevel);
			break;
		case ECellType::Bonus:
			RLGameMode->RequestSpawnHealItem();
			if (!GetCellInfo().SelectedBonusItem)
			{
				GetRLGameInst()->SetSelectedBonusItem(true);
				RLPlayerController->ShowSelectItemWidget(false);
			}
			CreateSidePortal();
			break;
		}
	}
}

void ARLMainGameState::KillBoss()
{
	CreateCenterPortal();
	check(RLPlayerController);
	RLPlayerController->ShowSelectItemWidget(false);
}

void ARLMainGameState::KillScored()
{
	if (++CurrentNum == ObjectiveNum)
	{
		ClearThisCell();
		CreateSidePortal();
	}
}

void ARLMainGameState::ClearThisCell()
{
	check(GetRLGameInst());
	GetRLGameInst()->ClearThisCell();
}

void ARLMainGameState::AteHealThisCell()
{
	check(GetRLGameInst());
	GetRLGameInst()->AteHealThisCell();
}

void ARLMainGameState::CreateSidePortal()
{
	check(RLGameMode);
	RLGameMode->CreateSidePortal();
}

void ARLMainGameState::CreateCenterPortal()
{
	check(RLGameMode);
	RLGameMode->CreateCenterPortal();
}

void ARLMainGameState::SpawnPrevBossPortal()
{
	check(RLGameMode);
	RLGameMode->CreatePrevBossPortal();
}