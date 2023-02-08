// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Roguelike/Type/DFSInterface.h"
#include "RLPlayerController.generated.h"


class ABSPAlgorithm;
class UMinimapWidget;
class APlayersCamera;

UCLASS()
class ROGUELIKE_API ARLPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	void SetMapInfo(FVector2Int MapSize, TArray<FCell> Board, int32 PlayerCurrentCell);
	void RemoveMinimapWidget();
	void ShowNoticeWidget();
	void ShowGameOverWidget();
protected:
	virtual void OnPossess(APawn* aPawn) override;
private:
	/* Map */
	TArray<FCell> Board;
	FVector2Int MapSize;
	int32 PlayerCell;
	void DrawMap();

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMinimapWidget> MinimapWidgetClass;
	UPROPERTY()
	UMinimapWidget* MinimapWidget;

	/****************************/

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> NoticeWidgetClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	/*이런저런 아이템 정보,플레이어 정보들*/


	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayersCamera> PlayersCameraClass;
public:
	
};
