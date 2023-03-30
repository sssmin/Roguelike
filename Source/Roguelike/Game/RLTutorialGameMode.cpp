// Fill out your copyright notice in the Description page of Project Settings.
#include "RLTutorialGameMode.h"

#include "TutorialManager.h"
#include "Blueprint/UserWidget.h"
#include "Roguelike/Actor/CellActor.h"
#include "Roguelike/Character/NormalMonster/MonsterCharacter.h"
#include "Roguelike/Component/ManagerComponent.h"
#include "Roguelike/Widget/TutorialWidget.h"


ARLTutorialGameMode::ARLTutorialGameMode()
{
}

void ARLTutorialGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TutorialManager = NewObject<UTutorialManager>(this);
	if (TutorialWidgetClass)
	{
		TutorialWidget = CreateWidget<UTutorialWidget>(GetWorld(), TutorialWidgetClass);
		if (TutorialWidget)
		{
			TutorialWidget->AddToViewport();
			TutorialWidget->VisibleDescWidget(false);
		}
	}
}

void ARLTutorialGameMode::SpawnTutorialMonster()
{
	TSubclassOf<AMonsterCharacter> SpawnMonsterClass = GetNormalMonsterClass(EKindOfMonster::Tutorial);
	AMonsterCharacter* Mob = GetWorld()->SpawnActor<AMonsterCharacter>(SpawnMonsterClass, GetMobSpawnPoints()[1], FRotator::ZeroRotator);
	 if (Mob && Mob->GetManagerComp())
	 {
	 	FHealthManager HealthManager;
	 	HealthManager.MaxHP = 99999.f;
	 	HealthManager.CurrentHP = 99999.f;
	 	FCombatManager CombatManager;
	 	
	 	Mob->GetManagerComp()->SetManager(HealthManager, CombatManager);
	 }
}

UTutorialManager* ARLTutorialGameMode::GetTutorialManager() const
{
	return TutorialManager;
}

void ARLTutorialGameMode::CreateTutorialPortal(uint8 Dir) const
{
	if (GetSpawnedCell())
	{
		GetSpawnedCell()->CreateTutorialPortal(Dir);
	}
}
