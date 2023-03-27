// Fill out your copyright notice in the Description page of Project Settings.
#include "TutorialMonster.h"

#include "Roguelike/Game/RLGameInstance.h"

void ATutorialMonster::OnHit(const FCombatManager& EnemyCombatManager, const FItemManager& EnemyItemManager,
                             AActor* Attacker, AActor* DamageCauser, TSubclassOf<UDamageType> DamageType)
{
	 URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	 if (GI)
	 {
	 	GI->GetListenerManager()->OnTutorialCompleted();
	 }
	//튜토 매니저한테 알려야할듯. 이게 첫번째몹방에도 잇을거고, 보스방에도 잇을건데
	//몹을 때렷을때 나오는 설명이나 나와야할 포탈과 보스를 때렷을 때 나오는설명이나 포탈이 다르기 땜에
	//이 몬스터를 생성할 때 그걸 구분할 수 잇는 변수가 잇어야할듯.
	//
}
