// Fill out your copyright notice in the Description page of Project Settings.
#include "TutorialManager.h"

#include "RLGameInstance.h"
#include "RLTutorialGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Roguelike/CustomBPFL.h"
#include "Roguelike/PlayerController/RLPlayerController.h"
#include "Roguelike/Widget/TutorialWidget.h"
#include "Roguelike/Widget/FadeWidget.h"

UTutorialManager::UTutorialManager()
{
	CellInfo = FCell();
	CurrentStep = 0;
	PrevEnterDir = 0;
}

void UTutorialManager::PostInitProperties()
{
	Super::PostInitProperties();
	
	TutorialGameMode = Cast<ARLTutorialGameMode>(UGameplayStatics::GetGameMode(this));
	GameInst = Cast<URLGameInstance>(UGameplayStatics::GetGameInstance(this));
	
	if (GameInst && GameInst->GetListenerManager())
	{
		GameInst->GetListenerManager()->OnStartTutorialDelegate.AddUObject(this, &ThisClass::StartTutorial);
		GameInst->GetListenerManager()->OnStepCompletedDelegate.BindUObject(this, &UTutorialManager::SetNextTutorial);
		GameInst->GetListenerManager()->OnTutorialCompletedDelegate.BindUObject(this, &UTutorialManager::TutorialCompleted);
	}
}

void UTutorialManager::StartTutorial()
{
	check(TutorialGameMode);
	check(GameInst);
	
	CellInfo = GameInst->GetCellInfo();
	TutorialGameMode->RequestSpawnCell(CellInfo.CellClass, CellInfo.TempWall);
	InitTutorialTable();
	InitDescTable();
	ExecuteTutorial();
}

void UTutorialManager::InitTutorialTable()
{
	if (Tutorials.IsEmpty())
	{
		const FString TutorialTablePath = FString(TEXT("/Game/DataTable/TutorialStep"));
		UDataTable* TutorialTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *TutorialTablePath));
		if (TutorialTableObject)
		{
			TArray<FTutorialStep*> Steps;
			TutorialTableObject->GetAllRows<FTutorialStep>(TEXT("AllSteps"), Steps);
			for (auto Step : Steps)
			{
				Tutorials.Add(UTutorialSteps::ConstructSteps(Step->TypeStep, Step->TutorialTypes));
			}
		}
	}
}

void UTutorialManager::InitDescTable()
{
	if (Descs.IsEmpty())
	{
		const FString TutorialDescTablePath = FString(TEXT("/Game/DataTable/TutorialDesc"));
		UDataTable* DescTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *TutorialDescTablePath));
		if (DescTableObject)
		{
			TArray<FTutorialDesc*> TutorialDescs;
			DescTableObject->GetAllRows<FTutorialDesc>(TEXT("AllDescs"), TutorialDescs);
			for (auto Desc : TutorialDescs)
			{
				Descs.Add(UTutorialDescs::ConstructDescs(Desc->Step, Desc->Desc));
			}
		}
	}
}

void UTutorialManager::EndTutorial()
{
	check(GameInst);
	UCustomBPFL::PlayLoadingScreen(GameInst->GetLoadingWidget(), true, 3.f);
	GameInst->NewGame();
	UGameplayStatics::OpenLevel(GetWorld(), "GameMap", true);
}

void UTutorialManager::ExecuteTutorial()
{
	check(TutorialGameMode);
	
	switch (Tutorials[CurrentStep]->TutorialTypes)
	{
	case ETutorialTypes::Desc:
		if (TutorialGameMode->GetTutorialWidget())
		{
			VisibleDesc(true);
			TutorialGameMode->GetTutorialWidget()->SetDescText(Descs[Tutorials[CurrentStep]->TypeStep - 1]->Desc);
		}
		break;
	case ETutorialTypes::SpawnMob:
		TutorialGameMode->SpawnTutorialMonster();
		break;
	case ETutorialTypes::ActivePortal:
		TutorialGameMode->CreateTutorialPortal(GetDir());
		break;
	case ETutorialTypes::ItemWidget:
		if (ARLPlayerController* PC = Cast<ARLPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			PC->ShowSelectItemWidget(true);
		}
		break;
	}
}

uint8 UTutorialManager::GetDir()
{
	TArray<uint8> Dirs;
	for (int32 i = 0; i < CellInfo.Status.Num(); ++i)
	{
		if (CellInfo.Status[i])
		{
			Dirs.Add(i);
		}
	}
	if (Dirs.Num() == 1)
	{
		PrevEnterDir = Dirs[0];
		return PrevEnterDir;
	}
	else
	{
		if (PrevEnterDir == 0)
		{
			return CalcConnectedDir(Dirs, 1);
		}
		else if (PrevEnterDir == 1)
		{
			return CalcConnectedDir(Dirs, 0);
		}
		else if (PrevEnterDir == 2)
		{
			return CalcConnectedDir(Dirs, 3);
		}
		else if (PrevEnterDir == 3)
		{
			return CalcConnectedDir(Dirs, 2);
		}
	}
	return 0;
}

uint8 UTutorialManager::CalcConnectedDir(TArray<uint8> Dirs, uint8 SideDir)
{
	for (int32 i = 0; i < Dirs.Num(); ++i)
	{
		if (Dirs[i] != SideDir)
		{
			PrevEnterDir = Dirs[i];
			return PrevEnterDir;
		}
	}
	return 0;
}

void UTutorialManager::DescTypingSkip()
{
	check(TutorialGameMode);
	TutorialGameMode->GetTutorialWidget()->TypingSkip();
}

bool UTutorialManager::CheckTypeCompleted()
{
	if (TutorialGameMode->GetTutorialWidget())
	{
		if (TutorialGameMode->GetTutorialWidget()->GetIsTypeCompleted())
		{
			return true;
		}
	}
	return false;
}

void UTutorialManager::VisibleDesc(bool Visible)
{
	if (TutorialGameMode->GetTutorialWidget())
	{
		TutorialGameMode->GetTutorialWidget()->VisibleDescWidget(Visible);
	}
}

void UTutorialManager::SetNextTutorial()
{
	CurrentStep++;
	ExecuteTutorial();
}

void UTutorialManager::TutorialCompleted()
{
	TSubclassOf<UFadeWidget> FadeWidgetClass = TutorialGameMode->GetFadeWidgetClass();
	if (FadeWidgetClass)
	{
		UFadeWidget* FadeWidget = CreateWidget<UFadeWidget>(GetWorld(), FadeWidgetClass);
		if (FadeWidget)
		{
			FadeWidget->AddToViewport();
			float EndTime = FadeWidget->PlayFadeOut();
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::EndTutorial, EndTime, false);
		}
	}
}

void UTutorialManager::ReconstructCell(uint8 Dir, const FCell& Info)
{
	check(TutorialGameMode);
	CellInfo = Info;
	TutorialGameMode->RequestSpawnCell(CellInfo.CellClass, CellInfo.TempWall, Dir);
	SetObjective();
}

void UTutorialManager::SetObjective()
{
	check(TutorialGameMode);

	if (CellInfo.CellType == ECellType::Bonus)
	{
		TutorialGameMode->RequestSpawnHealItem();
	}
}


