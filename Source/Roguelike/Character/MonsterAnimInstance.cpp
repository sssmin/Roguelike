// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Roguelike/Character/BossMonster/BossMonsterCharacter.h"

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Char = Cast<AMonsterCharacter>(TryGetPawnOwner());
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);


	if (!Char) Char = Cast<AMonsterCharacter>(TryGetPawnOwner());
	if (Char)
	{
		FVector Velocity = Char->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsAccelerating = Char->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	}
}

void UMonsterAnimInstance::AnimNotify_BreathStart()
{
	if (Cast<ABossMonsterCharacter>(Char))
	{
		Cast<ABossMonsterCharacter>(Char)->OnExecuteBreath();
	}
}

void UMonsterAnimInstance::AnimNotify_BreathForwardStart()
{
	if (Cast<ABossMonsterCharacter>(Char))
	{
		Cast<ABossMonsterCharacter>(Char)->OnExecuteBreathForward();
	}
}

void UMonsterAnimInstance::AnimNotify_ThrowBallStart()
{
	if (Cast<ABossMonsterCharacter>(Char))
	{
		Cast<ABossMonsterCharacter>(Char)->OnExecuteThrowBall();
	}
}

void UMonsterAnimInstance::AnimNotify_WhirlwindStart()
{
	if (Cast<ABossMonsterCharacter>(Char))
	{
		Cast<ABossMonsterCharacter>(Char)->OnExecuteWhirlwind();
	}
}

void UMonsterAnimInstance::AnimNotify_SquareStart()
{
	if (Cast<ABossMonsterCharacter>(Char))
	{
		Cast<ABossMonsterCharacter>(Char)->OnExecuteSquare();
	}
}

void UMonsterAnimInstance::AnimNotify_SkillEnd()
{
	if (Cast<ABossMonsterCharacter>(Char))
	{
		Cast<ABossMonsterCharacter>(Char)->OnSkillEnd();
	}
}
