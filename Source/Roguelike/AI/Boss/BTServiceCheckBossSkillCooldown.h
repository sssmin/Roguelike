// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceCheckBossSkillCooldown.generated.h"


UCLASS()
class ROGUELIKE_API UBTServiceCheckBossSkillCooldown : public UBTService
{
	GENERATED_BODY()
	
public:	
	UBTServiceCheckBossSkillCooldown();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UFUNCTION()
	void CooldownFinished(UBlackboardComponent* BBComp);
	void SetCooltime(UBlackboardComponent* BBComp);
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CanUseSkillKey;
	
	bool bSkillCooldown;
	float Cooltime;
	
};
