#pragma once
#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagSourceMode.h"
#include "BFl_TaggedComponent.generated.h"

UCLASS(BlueprintType)
class STARFIRE_API UBFl_TaggedComponent: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "TaggedComponent", meta = (WorldContext = "WorldContextObject"))
	static void GetAllActorsWithTag(
		UObject* WorldContextObject,
		TArray<AActor*>& OutActors,
		UPARAM(meta=(Categories="Gameplay.Actor")) const FGameplayTag Tag = FGameplayTag(),
		const EGameplayTagSourceMode SourceMode = EGameplayTagSourceMode::Both);

	UFUNCTION(BlueprintCallable, Category = "TaggedComponent", meta = (WorldContext = "WorldContextObject"))
	static bool GetFirstActorWithTag(
		UObject* WorldContextObject,
		AActor*& FoundActor,
		UPARAM(meta=(Categories="Gameplay.Actor")) const FGameplayTag Tag = FGameplayTag(),
		const EGameplayTagSourceMode SourceMode = EGameplayTagSourceMode::Both);
};
