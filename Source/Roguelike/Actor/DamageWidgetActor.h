// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageWidgetActor.generated.h"

class UWidgetComponent;
UCLASS()
class ROGUELIKE_API ADamageWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageWidgetActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void DestroyThisActor();
	void PlayNumWidget(bool IsPlayer, bool IsCritical, float Damage, bool IsHeal, bool IsDodge);
	
private:
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* NumWidgetComp;
	float DestroyTime;

public:
	UWidgetComponent* GetNumWidgetComp() const { return NumWidgetComp; }

};
