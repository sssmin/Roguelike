// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTaskMoveTo.generated.h"


UCLASS()
class ROGUELIKE_API UBTTaskMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
