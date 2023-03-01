// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Roguelike/Interface/DFSInterface.h"
#include "Roguelike/Type/ItemManage.h"
#include "RLPlayerController.generated.h"


DECLARE_DELEGATE(FRecallCompleteSuccessfully)

class APlayersCamera;
class UMinimapWidget;
class USelectItemWidget;
class UMainUIWidget;
class UNoticeWidget;
class URecallBarWidget;
class APlayerCharacter;
class UItemComponent;

UCLASS()
class ROGUELIKE_API ARLPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/* widget */
	UFUNCTION()
	void DeactiveOnceItemListWidget();
	void RemoveMinimapWidget();
	void ShowNoticeWidget(const FString& Notice);
	void ShowGameOverWidget();
	void ShowSelectItemWidget();
	void ActiveOnceItemListWidget(UItemInfo* SelectItem) const;
	void RemoveSelectWidget();
	void MoveMapFade();
	void InitOnceItemWidget();
	

	UFUNCTION(BlueprintCallable)
	void Init();
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime);

	void SetMapInfo(FVector2Int MapSize, TArray<FCell> Board, int32 PlayerCurrentCell);
	void ResumeController();
	void StopFire();
	
	void RegisterItemEmptySlot(UItemInfo* Item) const;
	UFUNCTION()
	void RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem);
	UFUNCTION()
	TArray<UItemInfo*> GetRandItem();

	void RecallStart();
	FRecallCompleteSuccessfully RecallCompleteSuccessfully;
protected:
	virtual void OnPossess(APawn* aPawn) override;
	void MoveForward(float Value);
	void MoveRight(float Value);
private:
	/* Map */
	TArray<FCell> Board;
	FVector2Int MapSize;
	int32 PlayerCell;
	void DrawMap();
	void ToggleMap();
	bool bVisibleMap;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Meta = (AllowPrivateAccess = "true"))
	FTimerHandle RecallTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Meta = (AllowPrivateAccess = "true"))
	float RecallTime;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<URecallBarWidget> RecallWidgetClass;
	UPROPERTY()
	URecallBarWidget* RecallBarWidget;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* RecallStartParticle;
	UPROPERTY()
	UParticleSystemComponent* RecallStartParticleComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* RecallEndParticle;

	UFUNCTION()
	void RecallTimerFinished();
	void RecallCancel();
	void RecallEnd();

	UFUNCTION()
	void RestorePC();

public:
	
};
