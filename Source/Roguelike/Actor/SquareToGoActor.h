// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BaseSkillActor.h"
#include "SquareToGoActor.generated.h"

UCLASS()
class ROGUELIKE_API ASquareToGoActor : public ABaseSkillActor
{
	GENERATED_BODY()
	
public:	
	ASquareToGoActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	

};
