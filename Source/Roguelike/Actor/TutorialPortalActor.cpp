// Fill out your copyright notice in the Description page of Project Settings.
#include "TutorialPortalActor.h"

#include "Roguelike/Game/RLGameInstance.h"

void ATutorialPortalActor::BeginOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlapped(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//포탈을 탔으니 다음 단계로.
	URLGameInstance* GI = URLGameInstance::GetRLGameInst(this);
	if (GI && GI->GetListenerManager())
	{
		GI->GetListenerManager()->OnStepCompleted();
	}
}
