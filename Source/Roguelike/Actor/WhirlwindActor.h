// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "BaseSkillActor.h"
#include "WhirlwindActor.generated.h"

class UDecalComponent;
class UParticleSystem;
class UBoxComponent;
class APlayerCharacter;

UCLASS()
class ROGUELIKE_API AWhirlwindActor : public ABaseSkillActor
{
	GENERATED_BODY()
	
public:	
	AWhirlwindActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UParticleSystem* WhirlwindParticle;

	UPROPERTY()
	UParticleSystemComponent* ParticleSystemComp;

	bool IsActive;
	float ActiveTime;
	float ApplyTime;
	float DestoryTime;
	void ActiveThisSkill();
	void DestroyThisSkill();

	UFUNCTION()
	void BoxBeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BoxEndOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY()
	APlayerCharacter* Player;
public:
};
