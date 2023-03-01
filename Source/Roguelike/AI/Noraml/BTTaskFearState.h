// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskFearState.generated.h"



UCLASS()
class ROGUELIKE_API UBTTaskFearState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskFearState();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FName LocKey;

};
