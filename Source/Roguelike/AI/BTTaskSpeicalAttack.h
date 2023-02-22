// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskSpeicalAttack.generated.h"


UCLASS()
class ROGUELIKE_API UBTTaskSpeicalAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskSpeicalAttack();
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FName TargetKey;
	FName CanSpecialAttackKey;
};
