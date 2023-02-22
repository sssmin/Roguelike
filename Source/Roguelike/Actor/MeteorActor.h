// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "MeteorActor.generated.h"

class UNiagaraComponent;
class UDecalComponent;

UCLASS()
class ROGUELIKE_API AMeteorActor : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
	
public:	
	AMeteorActor();
	
protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UDecalComponent* DecalComponent;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* MeteorNiagara;

	float MeteorTime;

	FTimerHandle MeteorTimerHandle;

	void Active();

};
