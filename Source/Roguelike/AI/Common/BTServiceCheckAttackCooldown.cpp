// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckAttackCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTServiceCheckAttackCooldown::UBTServiceCheckAttackCooldown()
{
	bSpecialAttackCooldown = false;
	Cooltime = 0.f;
}

void UBTServiceCheckAttackCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bSpecialAttackCooldown) return;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp)
	{
		bool CanSpecialAttack = BBComp->GetValueAsBool(CanSpecialAttackKey.SelectedKeyName);
		if (!CanSpecialAttack)
		{
			bSpecialAttackCooldown = true;
			SetRandCooltime(BBComp);
		}
	}
}

void UBTServiceCheckAttackCooldown::SetRandCooltime(UBlackboardComponent* BBComp)
{
	Cooltime = FMath::RandRange(2.f, 4.f);
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("CooldownFinished"), BBComp);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Cooltime, false);
}

void UBTServiceCheckAttackCooldown::CooldownFinished(UBlackboardComponent* BBComp)
{
	BBComp->SetValueAsBool(CanSpecialAttackKey.SelectedKeyName, true);
	bSpecialAttackCooldown = false;
}
