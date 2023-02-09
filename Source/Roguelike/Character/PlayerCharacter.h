// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;

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
	void GetElementFromItem(int32 ConvertElement);
	void IncreaseMovementSpeed();
	void DecreaseMovementSpeed();
	void HealByHit();
	void HealByItem();

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attack() override;

private:
	UPROPERTY()
	APlayerController* PC;
	FRotator LookRot;

	void TestIncreaseKillCount();
	void TestKillMe();
	void TestPrintMap();

	void Interact();
	void PressedFreeCam();
	void ReleasedFreeCam();

	bool bPressedAttackButton;
	float AttackDelay;

public:
	FRotator GetLookRot() const { return LookRot; }
	void SetLookRot(FRotator Rot) { LookRot = Rot; }
	
};
