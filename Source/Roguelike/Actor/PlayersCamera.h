// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayersCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;
class APlayerCharacter;

UCLASS()
class ROGUELIKE_API APlayersCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayersCamera();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	void ResetPlayerLocation();
	void FollowPlayer();
	void SetIsFreeCam(bool Boolean);
	void MoveCam();
	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CameraMoveCurve;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	
	FVector CurrentLoc;
	bool IsFreeCam;
	bool IsMoving;
	bool MoveCompleted;
	float CameraSpeed;
	FTimerHandle CameraMoveTimerHandle;
	
};
