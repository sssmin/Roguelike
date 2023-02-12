// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RLMonsterAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class ROGUELIKE_API ARLMonsterAIController : public AAIController
{
	GENERATED_BODY()
	public:
		ARLMonsterAIController();
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BBComp;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BTComp;
	
};
