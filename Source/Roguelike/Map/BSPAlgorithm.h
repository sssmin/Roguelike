// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BSPAlgorithm.generated.h"


DECLARE_DELEGATE(FOnCreateMapComplete);

struct FRect
{
	FRect() {}
	FRect(int32 InX, int32 InY, int32 InWidth, int32 InHeight) :X(InX), Y(InY), Width(InWidth), Height(InHeight) {}
	int32 X;
	int32 Y;
	int32 Width;
	int32 Height;
	FVector2D Center;
	
};

struct FRoomInfo
{
	FRoomInfo() {}
	FRoomInfo(TSharedPtr<FRect> InRect, FVector InRectLoc) :Rect(InRect), RectLoc(InRectLoc) {}
	TSharedPtr<FRect> Rect;
	FVector RectLoc;
};

USTRUCT()
struct FRLNode
{
	GENERATED_BODY()
	
	FRLNode() {}
	FRLNode(TSharedPtr<FRect> Rect):NodeRect(Rect) {}


	TSharedPtr<FRLNode> LeftNode;
	TSharedPtr<FRLNode> RightNode;
	TSharedPtr<FRLNode> ParNode;

	TSharedPtr<FRect> NodeRect;
	TSharedPtr<FRect> RoomRect;
	FIntVector2 GetCenter()
	{
		return FIntVector2(RoomRect->X + RoomRect->Width / 2, RoomRect->Y + RoomRect->Height / 2);
	}


};


UCLASS()
class ROGUELIKE_API ABSPAlgorithm : public AActor
{
	GENERATED_BODY()
	
public:	
	ABSPAlgorithm();
	virtual void Tick(float DeltaTime) override;
	void MakeMap();
	FOnCreateMapComplete OnCreateMapComplete;
	FVector TestPlayerLocBossLoc();
protected:
	virtual void BeginPlay() override;

private:	
	FIntVector2 MapSize;
	float MinDivideRate;
	float MaxDivideRate;
	int32 Type;
	int32 MaxDepth;

	TSharedPtr<FRLNode> Root;

	TMap<FVector, bool> BackgroundLoc;
	TMap<FVector, bool> RoomLoc; //바닥이 깔린 위치(복도 포함)
	TArray<FRoomInfo> RoomInfos;

	FIntVector2 MinXY;
	FIntVector2 MaxXY;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* ISMFloor;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* ISMLoad;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* ISMWall;


	void Divide(TSharedPtr<FRLNode> Tree, int32 N);
	TSharedPtr<FRect> GenerateRoom(TSharedPtr<FRLNode> Tree, int32 N);
	void GenerateLoad(TSharedPtr<FRLNode> Tree, int32 N);
	
	void FillBackground();
	void FillWall();
	void FillFloor(TSharedPtr<FRect> Rect);
	
	
};
