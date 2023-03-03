// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceCheckAttackCooldown.generated.h"


UCLASS()
class ROGUELIKE_API UBTServiceCheckAttackCooldown : public UBTService
{
	GENERATED_BODY()
public:
	UBTServiceCheckAttackCooldown();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UFUNCTION()
	void CooldownFinished(UBlackboardComponent* BBComp);
	void SetRandCooltime(UBlackboardComponent* BBComp);
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CanSpecialAttackKey;
	
	bool bSpecialAttackCooldown;
	float Cooltime;
	
};
