// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNodeCalcLocation.generated.h"

UCLASS()
class ROGUELIKE_API UBTTaskNodeCalcLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTaskNodeCalcLocation();

private:
	FName LocationKey;
	FName TargetKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
