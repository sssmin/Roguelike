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
	void GetElementFromItem(EElement Element) const;
	void IncreaseMovementSpeed() const;
	void DecreaseMovementSpeed() const;
	void HealByItem() const;
	void RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem) const;
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyElementParticle();
	void OnSkillHit(AActor* Attacker, AActor* DamageCauser, const FCombatManager& EnemyManager, TSubclassOf<UDamageType> DamageType);
	virtual void Dead() override;
	void StopFire();
	virtual void ShowNumWidget(float Damage, bool IsCritical, bool IsHeal, bool IsDodge) override;
	void ApplyMovementBuff() const;
	virtual void SetStateIcon(EState State) override;
	virtual void RemoveStateIcon(EState State) override;
	virtual void FlickerStateIcon(EState State) override;
	virtual void SetBuffIcon(EBuff Buff) override;
	virtual void RemoveBuffIcon(EBuff Buff) override;
	virtual void FlickerBuffIcon(EBuff Buff) override;
	
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
	void Dash();
	void CheckToGo(OUT float& Dist) const;
	float GetDistanceTo(FVector StandardLoc, FVector TargetLoc) const;
	
	
	UPROPERTY()
	APlayerController* PC;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UItemComponent* ItemComp;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UPlayerCombatComponent* PlayerCombatComp;
	
	FRotator LookRot;
	bool bPressedAttackButton;
	float MaxDashDist;
	
	void Test1();
	void Test2();
	void Test3();

	
public:
	FRotator GetLookRot() const { return LookRot; }
	void SetLookRot(FRotator Rot) { LookRot = Rot; }
	UFUNCTION(BlueprintPure)
	UItemComponent* GetItemComp() const { return ItemComp; }
	UFUNCTION(BlueprintPure)
	UPlayerCombatComponent* GetPlayerCombatComp() const { return PlayerCombatComp;}
	
};
