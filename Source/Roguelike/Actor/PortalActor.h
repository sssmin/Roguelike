// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Roguelike/Roguelike.h"
#include "GameFramework/Actor.h"
#include "Roguelike/Interface/InteractInterface.h"
#include "PortalActor.generated.h"

class USphereComponent;
class UParticleSystem;
class UWidgetComponent;

enum class EPortalType : uint8
{
	NONE,
	SIDE,
	CENTER,
	PREV_BOSS
};

UCLASS()
class ROGUELIKE_API APortalActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	APortalActor();
	void SetCenterPortal();
	void SetSidePortal();
	void SetPrevBossPortal();
	virtual void Interact() override;
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

	UFUNCTION()
	void InteractBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void InteractEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	EPortalType PortalType;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* InteractWidgetComp;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractSphereComp;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* InteractSphere;

	bool bIsInteractActive;
public:
	void PortalActivate();
	void SetDir(int32 InDir) { Dir = InDir; }
	EPortalType GetPortalType() const { return PortalType; }
	bool GetIsInteractActive() const { return bIsInteractActive; }
};
