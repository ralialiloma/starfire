// Copyright Phoenix Dawn Development LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractBase.generated.h"

UENUM(Blueprintable)
enum EInteractMoment
{
	Start,
	Tick,
	End
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UInteractBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "InMoment"))
	static EInteractMoment ConvertToInteractMoment(EInteractMoment InMoment);

};
