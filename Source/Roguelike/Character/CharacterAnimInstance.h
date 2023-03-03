// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"


class APlayerCharacter;

UCLASS()
class ROGUELIKE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float YawOffset;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsDead;
	UPROPERTY()
	APlayerCharacter* Char;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

public:
	void SetIsDead(bool Boolean) { bIsDead = Boolean; }
	
};
