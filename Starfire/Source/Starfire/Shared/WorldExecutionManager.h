#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldExecutionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllLevelsLoaded_BP);
DECLARE_MULTICAST_DELEGATE(FOnAllLevelsLoaded_CPP);

UCLASS()
class STARFIRE_API UWorldExecutionManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Delegate to notify when all levels and nested levels are fully loaded
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAllLevelsLoaded_BP OnAllLevelsLoaded_BP;
	FOnAllLevelsLoaded_CPP OnAllLevelsLoaded_CPP;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Check if all levels have already been loaded
	UFUNCTION(BlueprintCallable, Category = "Levels")
	bool HaveAllLevelsLoaded() const;

	// Helper function to register a callback
	void RegisterOnAllLevelsLoaded(const TFunction<void()>& Callback);

private:
	// Tracks the number of levels currently being loaded
	int32 NumLevelsLoading = 0;

	// Flag to track if the event has already been fired
	bool bAllLevelsLoaded = false;

	// Callback for when a level is added to the world
	void OnLevelAddedToWorld(ULevel* Level, UWorld* World);

	// Callback for when a level is shown
	UFUNCTION()
	void OnLevelShown();

	// Recursively track nested levels in a world
	void TrackNestedLevels(UWorld* World);

	// Checks if all levels are loaded and triggers the delegate if true
	void CheckAllLevelsLoaded();

	// Handles when all levels are loaded
	void AllLevelsLoaded();
};
