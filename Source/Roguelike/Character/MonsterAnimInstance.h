// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

class AMonsterCharacter;

UCLASS()
class ROGUELIKE_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UFUNCTION()
	void AnimNotify_BreathStart();
	UFUNCTION()
	void AnimNotify_BreathForwardStart();
	UFUNCTION()
	void AnimNotify_ThrowBallStart();
	UFUNCTION()
	void AnimNotify_WhirlwindStart();
	UFUNCTION()
	void AnimNotify_SquareStart();
	UFUNCTION()
	void AnimNotify_SkillEnd();
	
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float Speed;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool bIsDead;
	UPROPERTY()
	AMonsterCharacter* Char;

public:
	void SetIsDead(bool Boolean) { bIsDead = Boolean; }
	
};
