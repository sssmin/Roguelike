// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceCheckBossSkillCooldown.generated.h"


UCLASS()
class ROGUELIKE_API UBTServiceCheckBossSkillCooldown : public UBTService
{
	GENERATED_BODY()
	
	UBTServiceCheckBossSkillCooldown();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CanUseSkillKey;
	bool bSkillCooldown;
	float Cooltime;
	void SetCooltime(UBlackboardComponent* BBComp);
	UFUNCTION()
	void CooldownFinished(UBlackboardComponent* BBComp);
	
};
