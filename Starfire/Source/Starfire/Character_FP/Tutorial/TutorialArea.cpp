// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialArea.h"

#include "TutorialManager.h"
#include "Kismet/GameplayStatics.h"

void ATutorialArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!TutorialManager)
	{
		TutorialManager = Cast<ATutorialManager>(UGameplayStatics::GetActorOfClass(this, ATutorialManager::StaticClass()));

		if (!TutorialManager)
			return;
	}

	TutorialManager->EnterRegion(RegionTag);
}

void ATutorialArea::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	
}
