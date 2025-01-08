#include "ManagerActor.h"

#include "Starfire/Shared/WorldExecutionManager.h"

void AManagerActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UWorldExecutionManager>()->RegisterOnAllLevelsLoaded([this]()
		{
			StartGame();
		});
}

void AManagerActor::StartGame()
{	
	StartGameEvent();
}
