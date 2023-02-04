// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"
#include "Components/CapsuleComponent.h"

AMonsterCharacter::AMonsterCharacter()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
}
