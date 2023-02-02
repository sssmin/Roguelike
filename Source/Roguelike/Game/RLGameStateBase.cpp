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
//Ŭ���� ������ ���� �� ��Ż���� �����ؼ� ��ƼŬ ���̰�.

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

void ARLGameStateBase::ClearThisCell() //��� �������� Ŭ���� ���� ��
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