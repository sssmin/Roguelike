// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Roguelike/Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UItemComponent;
class UPlayerCombatComponent;

DECLARE_DELEGATE_OneParam(FOnPressedFreeCam, bool);

UCLASS()
class ROGUELIKE_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	virtual void BeginPlay() override;
	FOnPressedFreeCam OnPressedFreeCam;
	void GetElementFromItem(EElement Element);
	void IncreaseMovementSpeed();
	void DecreaseMovementSpeed();
	void HealByHit();
	void HealByItem();
	void RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem);
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyElementParticle();
	void OnSkillHit(AActor* Attacker, AActor* DamageCauser, const FCombatManager& EnemyManager, TSubclassOf<UDamageType> DamageType);
	virtual void Dead() override;
	void StopFire();
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack() override;

private:
	FItemManager GetItemManager() const;
	void AttackReleased();
	void Interact();
	void PressedFreeCam();
	void ReleasedFreeCam();
	void Recall();
	
	UPROPERTY()
	APlayerController* PC;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UItemComponent* ItemComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UPlayerCombatComponent* PlayerCombatComp;
	
	FRotator LookRot;
	bool bPressedAttackButton;
	
	void Test1();
	void Test2();
	void Test3();
	
public:
	FRotator GetLookRot() const { return LookRot; }
	void SetLookRot(FRotator Rot) { LookRot = Rot; }
	UItemComponent* GetItemComp() const { return ItemComp; }
	
	
};
