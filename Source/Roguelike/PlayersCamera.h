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
protected:
	virtual void BeginPlay() override;

public:	
	

private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	FVector CurrentLoc;
	bool IsFreeCam;
	bool IsMoving;
	bool MoveCompleted;
	float CameraSpeed;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CameraMoveCurve;
	FTimerHandle CameraMoveTimerHandle;

	void ResetPlayerLocation();
	void FollowPlayer();
	void SetIsFreeCam(bool Boolean);
	void MoveCam();


};
