// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNodeFire.generated.h"


UCLASS()
class ROGUELIKE_API UBTTaskNodeFire : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNodeFire();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
