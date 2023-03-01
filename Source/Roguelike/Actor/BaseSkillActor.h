// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Type/StatManage.h"
#include "BaseSkillActor.generated.h"

UCLASS()
class ROGUELIKE_API ABaseSkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSkillActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	FCombatManager OwnerCombatManager;
public:	
	void SetCombatManager(const FCombatManager& InManager) { OwnerCombatManager = InManager; }
	FCombatManager GetCombatManager() const { return OwnerCombatManager; }
};
