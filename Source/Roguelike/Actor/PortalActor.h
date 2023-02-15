// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Roguelike/Roguelike.h"
#include "GameFramework/Actor.h"
#include "PortalActor.generated.h"

class USphereComponent;
class UParticleSystem;

enum class EPortalType : uint8
{
	NONE,
	SIDE,
	CENTER,
	PREV_BOSS
};

UCLASS()
class ROGUELIKE_API APortalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APortalActor();
	void SetCenterPortal();
	void SetSidePortal();
	void SetPrevBossPortal();
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* PortalParticle;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* CenterPortalCreateParticle;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* CenterPortalParticle;

	UPROPERTY()
	UParticleSystemComponent* PortalParticleComp;

	UPROPERTY()
	UParticleSystemComponent* CenterPortalParticleComp;

	int32 Dir;

	UFUNCTION()
	void BeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	EPortalType PortalType;


public:
	void PortalParticleVisible(bool IsActive);
	void SetDir(int32 InDir) { Dir = InDir; }
	
};
