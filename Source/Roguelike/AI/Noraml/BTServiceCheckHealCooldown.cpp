// Fill out your copyright notice in the Description page of Project Settings.
#include "BTServiceCheckHealCooldown.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTServiceCheckHealCooldown::UBTServiceCheckHealCooldown()
{
	bHealCooldown = false;
	Cooltime = 10.f;
}

void UBTServiceCheckHealCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (bHealCooldown) return;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp)
	{
		bool CanHeal = BBComp->GetValueAsBool(CanHealKey.SelectedKeyName);
		if (!CanHeal)
		{
			bHealCooldown = true;
			SetCooltime(BBComp);
		}
	}
}

void UBTServiceCheckHealCooldown::SetCooltime(UBlackboardComponent* BBComp)
{
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("CooldownFinished"), BBComp);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Cooltime, false);
}

void UBTServiceCheckHealCooldown::CooldownFinished(UBlackboardComponent* BBComp)
{
	BBComp->SetValueAsBool(CanHealKey.SelectedKeyName, true);
	bHealCooldown = false;
}