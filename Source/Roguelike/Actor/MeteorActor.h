// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSkillActor.h"
#include "NiagaraDataInterfaceExport.h"
#include "MeteorActor.generated.h"

class UNiagaraComponent;
class UDecalComponent;

UCLASS()
class ROGUELIKE_API AMeteorActor : public ABaseSkillActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
	
public:	
	AMeteorActor();
	virtual void BeginPlay() override;
	
private:	
	void Active();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UDecalComponent* DecalComponent;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* MeteorNiagara;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	AActor* OwnerActor;
	
	float MeteorTime;
	
};
