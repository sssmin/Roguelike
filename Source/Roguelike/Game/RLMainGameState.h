// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "RLBaseGameState.h"
#include "RLMainGameState.generated.h"

class ARLPlayerController;
class ARLMainGameMode;
class URLGameInstance;


UCLASS()
class ROGUELIKE_API ARLMainGameState : public ARLBaseGameState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	ARLMainGameState();
	void KillScored();
	void KillBoss();
	void AteHealThisCell();
	void ReconstructCell(uint8 Dir, int32 Level, const FCell& Info);
	void NewGame();
	void LoadGame();
	void SpawnPrevBossPortal();

	
private:
	void ClearThisCell();
	void SetObjective();
	void CreateCenterPortal();
	void CreateSidePortal();

	UPROPERTY()
	ARLMainGameMode* RLGameMode;
	UPROPERTY()
	ARLPlayerController* RLPlayerController;
	
	int32 ObjectiveNum; //목표 킬수
	int32 CurrentNum; //현재 킬수
	int32 StageLevel;
	
public:
	
	
};
