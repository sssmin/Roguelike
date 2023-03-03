// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskSetMovementSpeed.generated.h"

UENUM()
enum class EMovementType : uint8
{
	DEFAULT,
	PATROL
};

UCLASS()
class ROGUELIKE_API UBTTaskSetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	EMovementType MovementType;
	
};
