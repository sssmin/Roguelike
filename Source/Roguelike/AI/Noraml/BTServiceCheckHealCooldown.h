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

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CanHealKey;
	
	bool bHealCooldown;
	float Cooltime;

	void SetCooltime(UBlackboardComponent* BBComp);
	UFUNCTION()
	void CooldownFinished(UBlackboardComponent* BBComp);
};
