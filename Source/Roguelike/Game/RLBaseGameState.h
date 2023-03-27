// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Roguelike/Interface/DFSInterface.h"
#include "RLBaseGameState.generated.h"


class ARLBaseGameMode;
class URLGameInstance;

UCLASS()
class ROGUELIKE_API ARLBaseGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ARLBaseGameState();
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	URLGameInstance* RLGameInst;
	FCell CellInfo;

public:
	FCell GetCellInfo() const { return CellInfo; }
	void SetCellInfo(const FCell& Info) { CellInfo = Info;  }
	URLGameInstance* GetRLGameInst() const { return RLGameInst; }
	
};
