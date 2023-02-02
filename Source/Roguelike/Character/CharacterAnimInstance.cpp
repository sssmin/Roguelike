// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "Roguelike/Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Char = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	
	if (!Char) Char = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (Char)
	{
		FVector Velocity = Char->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = Char->GetCharacterMovement()->IsFalling();
		bIsAccelerating = Char->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
		
		FRotator MovementRot = UKismetMathLibrary::MakeRotFromX(Char->GetVelocity());
		
		FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRot, Char->GetLookRot());
		

		DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
		
		YawOffset = DeltaRotation.Yaw;
	}
}