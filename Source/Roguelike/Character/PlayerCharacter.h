// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UItemComponent;

DECLARE_DELEGATE_OneParam(FOnPressedFreeCam, bool);

UCLASS()
class ROGUELIKE_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	FOnPressedFreeCam OnPressedFreeCam;
	void GetElementFromItem(EElement Element);
	void IncreaseMovementSpeed();
	void DecreaseMovementSpeed();
	void HealByHit();
	void HealByItem();
	void RequestItemSwap(const UItemInfo* OldItem, const UItemInfo* NewItem);
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyElementParticle();
protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack() override;

private:
	UPROPERTY()
	APlayerController* PC;
	FRotator LookRot;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UItemComponent* ItemComp;
	void Test1();
	void Test2();
	void Test3();

	void Interact();
	void PressedFreeCam();
	void ReleasedFreeCam();
	void Recall();

	bool bPressedAttackButton;
	FItemManager GetItemManager() const;

	void AttackReleased();
public:
	FRotator GetLookRot() const { return LookRot; }
	void SetLookRot(FRotator Rot) { LookRot = Rot; }
	UItemComponent* GetItemComp() const { return ItemComp; }
	void StopFire();
	
};
