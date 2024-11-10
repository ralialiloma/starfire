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
		GetWorld()->OnWorldBeginPlay.AddLambda([this]()
		{
			StartGame();
		});
	}
}

void UManagerComponent::StartGame()
{
	StartGameEvent();
}
