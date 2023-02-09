// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Roguelike/Type/DFSInterface.h"
#include "Roguelike/Type/Item.h"
#include "RLPlayerController.generated.h"


class UMinimapWidget;
class APlayersCamera;
class USelectItemWidget;
class APlayerCharacter;

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
	void ShowSelectItemWidget();
	virtual void PlayerTick(float DeltaTime);
	void ResumeController();
	void RemoveSelectWidget();
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
	void LookAtCursor();
	FRotator LookRot;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> NoticeWidgetClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USelectItemWidget> SelectItemWidgetClass;
	UPROPERTY()
	USelectItemWidget* CreatedSelectItemWidget;

	TArray<FAllItemTable> GetRandItem();
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayersCamera> PlayersCameraClass;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
public:
	
};
