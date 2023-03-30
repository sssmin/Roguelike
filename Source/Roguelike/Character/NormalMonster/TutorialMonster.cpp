// Fill out your copyright notice in the Description page of Project Settings.
#include "TutorialMonster.h"

#include "Roguelike/Game/RLGameInstance.h"

void ATutorialMonster::OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager, AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	 if (URLGameInstance* GI = URLGameInstance::GetRLGameInst(this))
	 {
	 	GI->GetListenerManager()->OnTutorialCompleted();
	 }
}
