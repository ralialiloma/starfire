#include "ManagerComponent.h"

void UManagerComponent::BeginPlay()
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

void UManagerComponent::StartGame()
{
	GetWorld()->OnWorldBeginPlay.Remove(StartGameDelegateHandle);
	
	StartGameEvent();
}
