// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "PortalActor.h"
#include "TutorialPortalActor.generated.h"


UCLASS()
class ROGUELIKE_API ATutorialPortalActor : public APortalActor
{
	GENERATED_BODY()

public:
	virtual void BeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	//오버랩됐을 때 처리가 달라야한다.
	
};
