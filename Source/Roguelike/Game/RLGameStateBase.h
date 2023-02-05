// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Roguelike/DFSInterface.h"
#include "RLGameStateBase.generated.h"



class UPortalComponent;

UCLASS()
class ROGUELIKE_API ARLGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	ARLGameStateBase();
	void KillScored();
	void KillBoss();
private:
	/* 셀 목표 */
	int32 ObjectiveNum; //목표 킬수
	int32 CurrentNum; //현재 킬수
	bool IsClear; //클리어인지

	/* 정보 */
	FCell CellInfo;
	int32 StageLevel;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UPortalComponent* PortalComp;

	void ClearThisCell();
	void SetObjective();
	void CreateCenterPortal();
	void ActivePortal();
public:
	void TestKillScored();
	
};
