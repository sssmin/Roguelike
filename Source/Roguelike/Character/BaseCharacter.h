// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCombatComponent;
class ABaseProjectile;
class UManagerComponent;

UCLASS()
class ROGUELIKE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void Attack();
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UManagerComponent* ManagerComp;

	
};
