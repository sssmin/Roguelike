// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Type/DFSInterface.h"
#include "CellActor.generated.h"

class UPortalComponent;

UCLASS()
class ROGUELIKE_API ACellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACellActor();
	virtual void Tick(float DeltaTime) override;
	void CreateSidePortal();
	void CreateCenterPortal();
	FVector GetPlayerSpawnLocation(int32 Dir);
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (Bitmask, BitmaskEnum = ETempWall, AllowPrivateAccess = "true"))
	uint8 TempWall;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UPortalComponent* PortalComp;

public:
	void SetTempWall(uint8 Value) { TempWall = Value; }
};
