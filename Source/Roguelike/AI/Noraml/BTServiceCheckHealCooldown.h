// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceCheckHealCooldown.generated.h"


UCLASS()
class ROGUELIKE_API UBTServiceCheckHealCooldown : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTServiceCheckHealCooldown();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UFUNCTION()
	void CooldownFinished(UBlackboardComponent* BBComp);
	void SetCooltime(UBlackboardComponent* BBComp);
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CanHealKey;
	
	bool bHealCooldown;
	float Cooltime;
	
};
