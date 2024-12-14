#include "ManagerActor.h"

void AManagerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld()->GetBegunPlay())
	{
		StartGame();
	}
	else
	{
		StartGameDelegateHandle = GetWorld()->OnWorldBeginPlay.AddLambda([this]()
		{
			StartGame();
		});
	}
}

void AManagerActor::StartGame()
{
	GetWorld()->OnWorldBeginPlay.Remove(StartGameDelegateHandle);
	
	StartGameEvent();
}
