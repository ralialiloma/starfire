#pragma once

#include "CoreMinimal.h"
#include "ManagerActor.generated.h"

UCLASS(Blueprintable)
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
