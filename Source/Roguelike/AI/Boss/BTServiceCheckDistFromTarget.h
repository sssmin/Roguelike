// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceCheckDistFromTarget.generated.h"


UCLASS()
class ROGUELIKE_API UBTServiceCheckDistFromTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTServiceCheckDistFromTarget();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
