// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Roguelike/Type/StatManage.h"
#include "Roguelike/Type/ItemManage.h"
#include "CombatComponent.generated.h"


DECLARE_DELEGATE_RetVal(FCombatManager, FGetCombatManager);
DECLARE_DELEGATE_RetVal(FItemManager, FGetItemManager);

class ABaseProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void BeginPlay() override;
	virtual void ReadyToFire(bool bPressed);
	FName GetRandomMontageSection();
	
	FGetCombatManager GetCombatManager;
	FGetItemManager GetItemManager;
	
protected:
	void PlayFireMontage();

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireMontage;

public:
	UAnimMontage* GetFireMontage() const { return FireMontage; }
	
};
