// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"


class USphereComponent;

UCLASS()
class ROGUELIKE_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
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
	void TestLevelMove();
	void TestPrintMap();

	void Interact();

public:
	FRotator GetLookRot() const { return LookRot; }
	
};
