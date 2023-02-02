// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Roguelike/Game/RLGameInstance.h"
#include "Roguelike/Game/RLGameStateBase.h"
#include "Roguelike/PlayerController/RLPlayerController.h"

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;


}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(0.f, 0.f, 0.f));
	PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		FInputModeGameAndUI InputModeData;
		PC->SetInputMode(InputModeData);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = GetActorLocation();
	FVector Target;
	FHitResult Hit;
	if (PC)
	{
		PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		if (Hit.bBlockingHit)
		{
			Target = Hit.ImpactPoint;
		}
	}

	LookRot = UKismetMathLibrary::FindLookAtRotation(Start, Target);
	LookRot.Yaw;
	SetActorRotation(FRotator(0.f, LookRot.Yaw, 0.f));
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("TestLevelMove", IE_Pressed, this, &ThisClass::TestLevelMove);
	PlayerInputComponent->BindAction("TestIncreaseKillCount", IE_Pressed, this, &ThisClass::TestIncreaseKillCount);
	PlayerInputComponent->BindAction("TestPrintMap", IE_Pressed, this, &ThisClass::TestPrintMap);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ThisClass::Attack);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ThisClass::MoveRight);
}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}


/****************************************
	Test Function
*/
void APlayerCharacter::TestIncreaseKillCount()
{
	ARLGameStateBase* GSB = Cast<ARLGameStateBase>(UGameplayStatics::GetGameState(this));
	if (GSB)
	{
		GSB->TestKillScored();
	}

}

void APlayerCharacter::TestLevelMove()
{
	PC->ClientTravel("/Game/Maps/GameMap1", ETravelType::TRAVEL_Absolute);
}

void APlayerCharacter::TestPrintMap()
{
	if (Cast<URLGameInstance>(GetWorld()->GetGameInstance()))
	{
		Cast<URLGameInstance>(GetWorld()->GetGameInstance())->TestPrintMap();
	}
}



void APlayerCharacter::Interact()
{
	//포탈에 다가가면 그냥 가지게할까?
}


void APlayerCharacter::Attack()
{
	Super::Attack();
}