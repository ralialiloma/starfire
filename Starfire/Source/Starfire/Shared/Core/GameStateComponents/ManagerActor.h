#pragma once

#include "CoreMinimal.h"
#include "ManagerActor.generated.h"

UCLASS(Blueprintable, HideCategories = ("Replication", "Rendering", "Collision", "Actor", "Input", "HLOD", "Physics", "LevelInstance", "Cooking", 
"WorldPartition", "DataLayers", "Networking"), AutoCollapseCategories = ("Actor Tick", "Events"), PrioritizeCategories = ("Settings"))
class STARFIRE_API AManagerActor : public AActor
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	virtual void StartGame();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "StartGame")
	void StartGameEvent();

private:
	FDelegateHandle StartGameDelegateHandle;

};
