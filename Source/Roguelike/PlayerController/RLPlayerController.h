// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Roguelike/Interface/DFSInterface.h"
#include "Roguelike/Type/ItemManage.h"
#include "Roguelike/Type/StatManage.h"
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
class UStatNoticeWidget;

UCLASS()
class ROGUELIKE_API ARLPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime);
	/* widget */
	UFUNCTION()
	void DeactivateOnceItemListWidget() const;
	void RemoveMinimapWidget() const;
	void ShowNoticeWidget(const FString& Notice) const;
	void ShowGameOverWidget();
	void ShowSelectItemWidget(bool IsTutorial);
	void DeactivateSelectItemWidget();
	void ActiveOnceItemListWidget(UItemInfo* SelectItem) const;
	void RemoveSelectWidget() const;
	void MoveMapFade() const;
	void InitOnceItemWidget() const;
	void SetStateIcon(EState State, UTexture2D* Icon) const;
	void RemoveStateIcon(EState State) const;
	void FlickerStateIcon(EState State) const;
	void SetBuffIcon(EBuff Buff, UTexture2D* Icon) const;
	void RemoveBuffIcon(EBuff Buff) const;
	void FlickerBuffIcon(EBuff Buff) const;
	void ShowStatNoticeWidget(EINFStackItem StatItem, const FString& StatName, const float CurrentValue, const float IncreaseValue);

	/*------------*/
	UFUNCTION(BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	void SetMapInfo(FVector2Int MapSize, TArray<FCell> Board, int32 PlayerCurrentCell);
	void ResumeController();
	void StopFire() const;
	void RegisterItemEmptySlot(UItemInfo* Item) const;
	UFUNCTION()
	void RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem) const;
	UFUNCTION()
	TArray<UItemInfo*> GetRandItem() const;
	void RecallStart();
	FRecallCompleteSuccessfully RecallCompleteSuccessfully;
	void LoadItem(TArray<UItemInfo*> ItemInfos);
	
protected:
	virtual void OnPossess(APawn* aPawn) override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	
private:
	UFUNCTION()
	void RecallTimerFinished();
	UFUNCTION()
	void RestorePC();
	void RecallCancel();
	void RecallEnd();
	void DrawMap();
	void ToggleMap();
	void LookAtCursor();

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMinimapWidget> MinimapWidgetClass;
	UPROPERTY()
	UMinimapWidget* MinimapWidget;
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
	UPROPERTY()
	TArray<FCell> Board;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UStatNoticeWidget> StatNoticeClass;
	
	FVector2Int MapSize;
	int32 PlayerCell;
	bool bVisibleMap;
	
	
};
