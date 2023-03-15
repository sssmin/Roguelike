// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Roguelike/Interface/DFSInterface.h"
#include "RLGameStateBase.generated.h"

class ARLPlayerController;
class ARLGameModeBase;
class URLGameInstance;


UCLASS()
class ROGUELIKE_API ARLGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	ARLGameStateBase();
	void KillScored();
	void KillBoss();
	void SpawnCell(uint8 Dir);
	void AteHealThisCell();
	void ReconstructCuzMove(uint8 Dir, int32 Level, const FCell& Info);
	void NewGame();
	void LoadGame();
	void SpawnPrevBossPortal();
	
private:
	void ClearThisCell();
	void SetObjective();
	void CreateCenterPortal();
	void CreateSidePortal();

	UPROPERTY()
	ARLGameModeBase* RLGameMode;
	UPROPERTY()
	URLGameInstance* RLGameInst;
	UPROPERTY()
	ARLPlayerController* RLPlayerController;
	
	int32 ObjectiveNum; //목표 킬수
	int32 CurrentNum; //현재 킬수
	
	FCell CellInfo;
	int32 StageLevel;
	
public:
	void SetCellInfo(const FCell& Info) { CellInfo = Info;  }
	
};
