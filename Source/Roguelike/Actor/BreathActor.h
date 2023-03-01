// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkillActor.h"
#include "BreathActor.generated.h"

class UBoxComponent;
class APlayerCharacter;

UCLASS()
class ROGUELIKE_API ABreathActor : public ABaseSkillActor
{
	GENERATED_BODY()
	
public:	
	ABreathActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	float ActiveTime;
	bool IsActive;
	float ApplyTime;
	UPROPERTY()
	APlayerCharacter* Player;

	UFUNCTION()
	void OnBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
