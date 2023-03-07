// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BaseSkillActor.h"
#include "BreathActor.generated.h"

class UBoxComponent;
class APlayerCharacter;

UCLASS()
class ROGUELIKE_API ABreathActor : public ABaseSkillActor
{
	GENERATED_BODY()
	
public:	
	ABreathActor();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	void OnBeginOverlapped();
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	
	float ActiveTime;
	
};
