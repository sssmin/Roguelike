// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Roguelike/Type/DFSInterface.h"
#include "Roguelike/Type/ItemManage.h"
#include "RLPlayerController.generated.h"


class UMinimapWidget;
class APlayersCamera;
class USelectItemWidget;
class UMainUIWidget;
class UNoticeWidget;
class APlayerCharacter;

UCLASS()
class ROGUELIKE_API ARLPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/* widget */
	void RemoveMinimapWidget();
	void ShowNoticeWidget(const FString& Notice);
	void ShowGameOverWidget();
	void ShowSelectItemWidget();
	void ActiveOnceItemListWidget(FItemInfoTable* SelectItem);
	void DeactiveOnceItemListWidget();
	void RemoveSelectWidget();
	void MoveMapFade();

	UFUNCTION(BlueprintCallable)
	void Init();
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime);

	void SetMapInfo(FVector2Int MapSize, TArray<FCell> Board, int32 PlayerCurrentCell);
	void ResumeController();
	
	void RegisterItemEmptySlot(FItemInfoTable* Item);
	void RequestItemSwap(const FItemInfoTable* OldItem, const FItemInfoTable* NewItem);
	TArray<FItemInfoTable> GetRandItem();
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
	TSubclassOf<UNoticeWidget> NoticeWidgetClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USelectItemWidget> SelectItemWidgetClass;
	UPROPERTY()
	USelectItemWidget* CreatedSelectItemWidget;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;
	UPROPERTY()
	UMainUIWidget* MainUIWidget;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> FadeWidgetClass;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayersCamera> PlayersCameraClass;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	UPROPERTY()
	APlayersCamera* CurrentPlayersCamera;

public:
	
};
