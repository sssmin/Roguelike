// Fill out your copyright notice in the Description page of Project Settings.


#include "RLGameStateBase.h"
#include "RLGameInstance.h"
#include "RLGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Roguelike/Component/PortalComponent.h"

ARLGameStateBase::ARLGameStateBase()
{
	PortalComp = CreateDefaultSubobject<UPortalComponent>(TEXT("PortalComponent"));

	ObjectiveNum = 0;
	CurrentNum = 0;
	IsClear = false;
	StageLevel = 0;
}

void ARLGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	URLGameInstance* RLGameInstance = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (RLGameInstance)
	{
		StageLevel = RLGameInstance->GetStageLevel();
		CellInfo = RLGameInstance->GetCellInfo();
		SetObjective();
	}

}
//클리어 조건이 됐을 때 포탈컴프 접근해서 파티클 보이게.

void ARLGameStateBase::SetObjective()
{
	ARLGameModeBase* GM = Cast<ARLGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (CellInfo.IsCleared)
	{
		ActivePortal();
	}
	else
	{
		switch (CellInfo.CellType)
		{
		case ECellType::MOBS:
			if (GM)
			{
				ObjectiveNum = FMath::RandRange(4, 6);
				GM->SpawnMob(StageLevel, ObjectiveNum);
			}
			break;
		case ECellType::BOSS:
			if (GM)
			{
				GM->SpawnBoss(StageLevel);
			}
			break;
		case ECellType::BONUS: 
			ClearThisCell();  
			break;
		}
	}
	
	
}

void ARLGameStateBase::KillScored()
{
	if (++CurrentNum == ObjectiveNum)
	{
		IsClear = true;
		ClearThisCell();
	}
}

void ARLGameStateBase::ClearThisCell() //어떠한 조건으로 클리어 했을 때
{
	URLGameInstance* RLGameInstance = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (RLGameInstance)
	{
		RLGameInstance->ClearThisCell();
	}

	ActivePortal();
}

void ARLGameStateBase::TestKillScored()
{
	UE_LOG(LogTemp, Warning, TEXT("CurrentNum : %d, ObjectiveNum : %d"), CurrentNum, ObjectiveNum);
	if (++CurrentNum == ObjectiveNum)
	{
		ClearThisCell();
	}
}

void ARLGameStateBase::ActivePortal()
{
	if (PortalComp)
	{
		PortalComp->ActiveAllPortal();
	}
}