// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Roguelike/Type/DFSInterface.h"
#include "RLGameStateBase.generated.h"

class ARLGameModeBase;



UCLASS()
class ROGUELIKE_API ARLGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	ARLGameStateBase();
	void KillScored();
	void KillBoss();
	void SpawnCell(int32 Dir);
	void AteHealThisCell();
	void ReconstructCuzMove(int32 Dir, int32 Level, const FCell& Info);
	UFUNCTION(BlueprintCallable)
	void Init();
private:
	/* �� ��ǥ */
	int32 ObjectiveNum; //��ǥ ų��
	int32 CurrentNum; //���� ų��

	/* ���� */
	FCell CellInfo;
	int32 StageLevel;

	UPROPERTY()
	ARLGameModeBase* RLGameMode;

	void ClearThisCell();
	void SetObjective();
	void CreateCenterPortal();
	void CreateSidePortal();
public:
	void TestKillScored();
	void SetCellInfo(const FCell& Info) { CellInfo = Info;  }
	
};
