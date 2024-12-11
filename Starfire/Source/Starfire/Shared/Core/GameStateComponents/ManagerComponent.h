#pragma once

#include "CoreMinimal.h"
#include "ManagerComponent.generated.h"

UCLASS(Blueprintable)
class STARFIRE_API UManagerComponent : public UActorComponent
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
