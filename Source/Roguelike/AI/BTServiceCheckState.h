// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Roguelike/Type/StatManage.h"
#include "BTServiceCheckState.generated.h"

UENUM(BlueprintType)
enum MonsterState
{
	DEFAULT,
	BURN,
	FROZEN,
	POISON,
	FEAR,
	DEAD,

	MAX
};

UCLASS()
class ROGUELIKE_API UBTServiceCheckState : public UBTService
{
	GENERATED_BODY()
public:
	UBTServiceCheckState();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

private:
	void CheckState(UBehaviorTreeComponent& OwnerComp);

	UPROPERTY(Meta = (Bitmask, BitmaskEnum = EState))
	uint8 CurrentState;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector StateKey;
};
