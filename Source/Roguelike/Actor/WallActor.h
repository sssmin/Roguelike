// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallActor.generated.h"

UCLASS()
class ROGUELIKE_API AWallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWallActor();
	void SetSM(int32 CellClass);
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	UStaticMeshComponent* SMComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMesh*> Walls;

};
