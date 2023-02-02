// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DFSInterface.h"
#include "DFSAlgorithm.generated.h"



UCLASS()
class ROGUELIKE_API ADFSAlgorithm : public AActor
{
	GENERATED_BODY()
	
public:	
	ADFSAlgorithm();
	FVector StartAlgorithm();
protected:
	virtual void BeginPlay() override;

private:
	
	void MazeGenerator();
	void GenerateDungeon();
	void Init(); //갈래당 방 갯수(시작점 제외), 시작점, 갈래길 갯수 세팅, 시작점 갈래길 저장
	TArray<int32> CheckSideCell(int32 Cell);

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	FVector2Int Size;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	int32 StartPos;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	FVector2Int Offset;
	TArray<FCell> Board;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<FRule> Rooms;
	int32 RoomCount;
	int32 Parts; //시작점에서 몇갈래? 랜덤
	int32 CreatedPart;
	int32 BossCell;
	FVector StartPostion;
public:
	FVector GetStartPostion() const { return StartPostion; }

};
