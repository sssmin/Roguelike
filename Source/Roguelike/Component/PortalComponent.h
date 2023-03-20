// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PortalComponent.generated.h"


class APortalActor;
class ATutorialPortalActor;

USTRUCT()
struct FPortalInfo
{
	GENERATED_BODY()
	FPortalInfo() : Dir(0), Portal(nullptr) {}
	FPortalInfo(int32 InDir, APortalActor* InPortal) : Dir(InDir), Portal(InPortal) {}

	int32 Dir;
	UPROPERTY()
	APortalActor* Portal;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UPortalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPortalComponent();
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CreateSidePortal();
	void CreateCenterPortal();
	void CreatePrevBossPortal();
	void DestroyPortal();
	FVector GetArrowLocation(uint8 Dir);
	void CreateTutorialPortal(uint8 Dir);

private:
	void SetLocationPotal();
	void CreatePortal(TArray<uint8> Dirs);
	FVector CalcLocation(uint8 Dir);

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APortalActor> PortalActorClass;
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATutorialPortalActor> TutorialPortalActorClass;
	UPROPERTY()
	APortalActor* PrevBossPortal;
	UPROPERTY()
	TArray<FPortalInfo> Portals;
	
};
