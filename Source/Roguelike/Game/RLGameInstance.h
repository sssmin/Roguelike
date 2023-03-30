// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Roguelike/Interface/DFSInterface.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "Roguelike/Game/RLListenerManager.h"
#include "RLGameInstance.generated.h"


UENUM(BlueprintType)
enum class ELoadState : uint8
{
	None,
	NewGame,
	LoadGame,
	LevelTravel,
	Tutorial
};

class URoguelikeSaveGame;
class DFSAgrt;
class ARLMainGameState;
class ARLMainGameMode;
struct FInfoRecord;

DECLARE_DELEGATE(FOnInitOnceItem)
DECLARE_MULTICAST_DELEGATE(FSetTempManage)

UCLASS()
class ROGUELIKE_API URLGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	URLGameInstance();
	virtual void Init() override;
	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;
	UFUNCTION(BlueprintCallable)
	void NewGame();
	UFUNCTION(BlueprintCallable)
	void StartTutorial();
	UFUNCTION(BlueprintPure)
	bool CanSaveThisCell();
	UFUNCTION(BlueprintCallable)
	void SaveGame(UPARAM(ref) FTransform& PlayerTransform);
	UFUNCTION(BlueprintCallable)
	void LoadGame();
	UFUNCTION(BlueprintCallable)
	bool CanBind(FName MappingName, FKey Key);
	UFUNCTION(BlueprintCallable)
	void RemoveBindKey(FKey Key);
	UFUNCTION(BlueprintCallable)
	void ChangeBindKey(FName MappingName, FKey Key, float ValueIfAxis);
	UFUNCTION(BlueprintCallable)
	void RollbackBindKey(FName MappingName, FKey Key, float ValueIfAxis);
	
	void RequestInfo() const;
	void RequestMove(uint8 Dir);
	void RequestMoveNextStage();
	void RequestMovePrevBossCell();
	TArray<uint8> GetConnectedDir();
	void ClearThisCell();
	void AteHealThisCell();
	void ReadyToRecall();
	void GetManager(OUT FHealthManager& OutHealthManager, OUT FCombatManager& OutCombatManager, OUT uint8& OutBuff) const;
	void GetManager(OUT FItemManager& OutItemManager, OUT TMap<uint8, uint8>& OutFixMaxNum, OUT TArray<UItemInfo*>& OutItemInfos) const;
	void SetManager(const FHealthManager& InHealthManager, const FCombatManager& InCombatManager, const uint8& InBuff);
	void SetManager(const FItemManager& InItemManager, const TMap<uint8, uint8>& InFixMaxNum, const TArray<UItemInfo*>& InItemInfos);
	void SetSelectedBonusItem(bool Boolean);
	FCell GetCellInfo() const ;
	UFUNCTION(BlueprintCallable)
	URLListenerManager* GetListenerManager() const;
	
	

	FOnInitOnceItem InitOnceItemDelegate;
	FSetTempManage SetTempManageDelegate;

	void TestPrintMap();
	static URLGameInstance* GetRLGameInst(const UObject* WorldContextObject);
	
private:
	int32 CalcNextCell(uint8 Dir) const;
	void CheckEarlyDiscoveredBossCell();
	void Recall();
	void MoveProcess(int32 TargetCell, uint8 Dir);
	void LoadDataSetting(const FInfoRecord& Record);
	void RemoveNoneAxis(FName MappingName);
	void Initialize(int32 Level);
	void GenerateMap();
	void GenerateTutorialMap();
	
	TSharedPtr<DFSAgrt> DFS;
	UPROPERTY()
	ARLMainGameState* RLGameState;
	UPROPERTY()
	ARLMainGameMode* RLGameMode;
	UPROPERTY()
	URLListenerManager* ListenerManager;
	UPROPERTY(BlueprintReadWrite,  Meta = (AllowPrivateAccess = "true"))
	UUserWidget* LoadingWidget;

	/*
	 *  GameInstance에서 관리 또는 저장 시 필요한 변수들
	 */
	
	UPROPERTY()
	TArray<FCell> Board;
	UPROPERTY()
	URoguelikeSaveGame* RLSaveGame;
	UPROPERTY()
	TArray<UItemInfo*> TempItemInfos;
	UPROPERTY()
	TMap<uint8, uint8> TempFixMaxNum;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 StageLevel; 
	
	FVector2Int MapSize;
	int32 ClearCount; //방 클리어 갯수
	FVector StartPos;
	int32 StartCell;
	int32 PlayerCurrentCell;
	int32 BossCell;
	int32 BossPrevCell;
	int32 TotalCellNum; // 방 총개수. 시작지점 미포함. Total - 1 == ClearCount면 보스 입장 가능
	bool bIsEarlyDiscoveredBoss;
	
	FHealthManager TempHealthManager;
	FCombatManager TempCombatManager;
	FItemManager TempItemManager;
	uint8 TempBuff;
	FTransform TempPlayerTransform;
	int32 TempDashChargeNum;

	/*******************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	ELoadState LoadState;
	int32 AxisRollbackCount;
	
	
public:
	int32 GetStageLevel() const { return StageLevel; }
	TArray<UItemInfo*> GetItemInfos() const { return TempItemInfos; }
	FTransform GetTempPlayerTransform() const { return TempPlayerTransform; }
	void SetTempDashChargeNum(int32 InNum) { TempDashChargeNum = InNum; }
	int32 GetTempDashChargeNum() const { return TempDashChargeNum; }
	void SetLoadState(ELoadState InLoadState) { LoadState = InLoadState; }
	UUserWidget* GetLoadingWidget() const { return LoadingWidget; }
	
};
