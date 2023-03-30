// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckBossSkillCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTServiceCheckBossSkillCooldown::UBTServiceCheckBossSkillCooldown()
{
	Cooltime = 0.f;
	bSkillCooldown = false;
}

void UBTServiceCheckBossSkillCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bSkillCooldown) return;

	if (UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent())
	{
		bool CanUseSkill = BBComp->GetValueAsBool(CanUseSkillKey.SelectedKeyName);
		if (!CanUseSkill)
		{
			bSkillCooldown = true;
			SetCooltime(BBComp);
		}
	}
}

void UBTServiceCheckBossSkillCooldown::SetCooltime(UBlackboardComponent* BBComp)
{
	FTimerHandle TimerHandle;
	Cooltime = FMath::RandRange(2.f, 3.f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, BBComp]()
		{
			CooldownFinished(BBComp);
		}, Cooltime, false);
}

void UBTServiceCheckBossSkillCooldown::CooldownFinished(UBlackboardComponent* BBComp)
{
	BBComp->SetValueAsBool(CanUseSkillKey.SelectedKeyName, true);
	bSkillCooldown = false;
}
