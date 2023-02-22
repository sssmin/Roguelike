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
	
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CanSpecialAttackKey;


	bool bSpecialAttackCooldown;
	float Cooltime;
	
	void SetRandCooltime(UBlackboardComponent* BBComp);
	UFUNCTION()
	void CooldownFinished(UBlackboardComponent* BBComp);
};
