// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayersCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Roguelike/Character/Player/PlayerCharacter.h"


APlayersCamera::APlayersCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	RootComponent = SpringArm;
	SpringArm->TargetArmLength = 2000.f;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	IsFreeCam = false;
	IsMoving = false;
	MoveCompleted = false;
	CameraSpeed = 20.f;
}

void APlayersCamera::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	SetActorLocation(PlayerCharacter->GetActorLocation());
	EnableInput(Cast<APlayerController>(PlayerCharacter->GetController()));
	if (PlayerCharacter)
	{
		PlayerCharacter->OnPressedFreeCam.BindUObject(this, &ThisClass::SetIsFreeCam);
	}
}

void APlayersCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsFreeCam) //우클릭 중
	{
		MoveCompleted = false;
		IsMoving = false;
		MoveCam();
	}
	else //우클릭 뗌
	{
		if (!MoveCompleted)
		{
			if (!IsMoving) //떼고나서 한번만 실행해야함
			{
				if (GetWorld())
				{
					GetWorld()->GetTimerManager().SetTimer(CameraMoveTimerHandle, 0.5f, false);
					IsMoving = true;
				}
			}
			ResetPlayerLocation();
		}
		else
		{
			FollowPlayer();
		}
	}
}

void APlayersCamera::FollowPlayer()
{
	SetActorLocation(PlayerCharacter->GetActorLocation());
}

void APlayersCamera::SetIsFreeCam(bool Boolean)
{
	IsFreeCam = Boolean;
}

void APlayersCamera::ResetPlayerLocation()
{
	if (!UKismetMathLibrary::EqualEqual_VectorVector(GetActorLocation(), PlayerCharacter->GetActorLocation(), 1.f))
	{
		CurrentLoc = GetActorLocation();
		if (CameraMoveCurve && GetWorld())
		{
			const float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(CameraMoveTimerHandle);
			const float CurveValue = CameraMoveCurve->GetFloatValue(ElapsedTime);

			FVector ResultVector = FMath::Lerp(CurrentLoc, PlayerCharacter->GetActorLocation(), CurveValue);
			SetActorLocation(ResultVector);
			if (GetWorld()->GetTimerManager().GetTimerRemaining(CameraMoveTimerHandle) <= 0.f)
			{
				MoveCompleted = true;
				IsMoving = false;
			}
		}
	}
}

void APlayersCamera::MoveCam()
{
	FVector SubLoc = UKismetMathLibrary::Subtract_VectorVector(GetActorLocation(), PlayerCharacter->GetActorLocation());
	const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	if (SubLoc.X <= 500.f) //뷰포트 상단 500까지 갈 수 있음
	{
		float ViewportRange = UKismetMathLibrary::NormalizeToRange(MousePos.Y * ViewportScale, 0.f, ViewportSize.Y * 0.1f);
		ViewportRange = FMath::Clamp(ViewportRange, 0.f, 1.f);

		const FVector DeltaLoc{ (CameraSpeed) * (1.f - ViewportRange), 0.f , 0.f };
		RootComponent->AddRelativeLocation(DeltaLoc);
	}
	if (SubLoc.X >= -500.f) 
	{
		float ViewportRange = UKismetMathLibrary::NormalizeToRange(MousePos.Y * ViewportScale, ViewportSize.Y * 0.9f, ViewportSize.Y);
		ViewportRange = FMath::Clamp(ViewportRange, 0.f, 1.f);

		const FVector DeltaLoc{ -(CameraSpeed * 1.f) * (ViewportRange), 0.f , 0.f };
		RootComponent->AddRelativeLocation(DeltaLoc);
	}
	if (SubLoc.Y <= 500.f)
	{
		float ViewportRange = UKismetMathLibrary::NormalizeToRange(MousePos.X * ViewportScale, ViewportSize.X * 0.9f, ViewportSize.X);
		ViewportRange = FMath::Clamp(ViewportRange, 0.f, 1.f);

		const FVector DeltaLoc{ 0.f, (CameraSpeed) * (ViewportRange), 0.f };
		RootComponent->AddRelativeLocation(DeltaLoc);
	}
	if (SubLoc.Y >= -500.f)
	{
		float ViewportRange = UKismetMathLibrary::NormalizeToRange(MousePos.X * ViewportScale, 0.f, ViewportSize.X * 0.1f);
		ViewportRange = FMath::Clamp(ViewportRange, 0.f, 1.f);

		const FVector DeltaLoc{ 0.f, (CameraSpeed * -1.f) * (1.f - ViewportRange), 0.f };
		RootComponent->AddRelativeLocation(DeltaLoc);
	}
}