#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldExecutionManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWorldExecutionManager, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllLevelsLoaded_BP);
DECLARE_MULTICAST_DELEGATE(FOnAllLevelsLoaded_CPP);

UCLASS()
class STARFIRE_API UWorldExecutionManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAllLevelsLoaded_BP OnAllLevelsLoaded_BP;
	FOnAllLevelsLoaded_CPP OnAllLevelsLoaded_CPP;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category = "Levels")
	bool HaveAllLevelsLoaded() const;
	
	void RegisterOnAllLevelsLoaded(const TFunction<void()>& Callback);

private:
	
	int32 NumLevelsFound = 0;
	int32 NumLevelsLoading = 0;
	bool bAllLevelsLoaded = false;

	void OnLevelAddedToWorld(ULevel* Level, UWorld* World);
	
	UFUNCTION()
	void OnLevelShown();
	void TrackNestedLevels(UWorld* World);
	void CheckAllLevelsLoaded();
	void AllLevelsLoaded();
};
