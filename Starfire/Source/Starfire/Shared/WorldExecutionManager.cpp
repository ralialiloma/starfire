#include "WorldExecutionManager.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void UWorldExecutionManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Bind to the LevelAddedToWorld delegate to track dynamically added levels
    FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UWorldExecutionManager::OnLevelAddedToWorld);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("WorldExecutionManager initialized."));
}

void UWorldExecutionManager::Deinitialize()
{
    // Unbind the LevelAddedToWorld delegate
    FWorldDelegates::LevelAddedToWorld.RemoveAll(this);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WorldExecutionManager deinitialized."));

    Super::Deinitialize();
}

bool UWorldExecutionManager::HaveAllLevelsLoaded() const
{
    return bAllLevelsLoaded;
}

void UWorldExecutionManager::RegisterOnAllLevelsLoaded(const TFunction<void()>& Callback)
{
    if (HaveAllLevelsLoaded())
    {
        // If all levels are already loaded, call the callback immediately
        Callback();

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("All levels are already loaded. Callback invoked immediately."));
    }
    else
    {
        // Otherwise, bind a one-time delegate to call the callback
        OnAllLevelsLoaded_CPP.AddLambda(Callback);

        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Callback registered for OnAllLevelsLoaded."));
    }
}

void UWorldExecutionManager::OnLevelAddedToWorld(ULevel* Level, UWorld* World)
{
    if (World == GetWorld() && Level)
    {
        FString LevelName = Level->GetOuter()->GetName();
        UE_LOG(LogTemp, Log, TEXT("Level added to world: %s"), *LevelName);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Level added to world: %s"), *LevelName));

        TrackNestedLevels(World);
    }
}

void UWorldExecutionManager::TrackNestedLevels(UWorld* World)
{
    if (!World)
    {
        return;
    }

    for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
    {
        if (StreamingLevel)
        {
            FString StreamingLevelName = StreamingLevel->GetWorldAssetPackageName();
            // If the level is not yet loaded, subscribe to its OnLevelShown event
            if (!StreamingLevel->HasLoadedLevel())
            {
                NumLevelsLoading++;
                StreamingLevel->OnLevelShown.AddDynamic(this, &UWorldExecutionManager::OnLevelShown);

                UE_LOG(LogTemp, Log, TEXT("Tracking nested level: %s"), *StreamingLevelName);
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Tracking nested level: %s"), *StreamingLevelName));
            }
        }
    }

    CheckAllLevelsLoaded();
}

void UWorldExecutionManager::OnLevelShown()
{
    NumLevelsLoading--;

    UE_LOG(LogTemp, Log, TEXT("A level finished loading. Remaining: %d"), NumLevelsLoading);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("A level finished loading. Remaining: %d"), NumLevelsLoading));

    CheckAllLevelsLoaded();
}

void UWorldExecutionManager::CheckAllLevelsLoaded()
{
    if (NumLevelsLoading <= 0 && !bAllLevelsLoaded)
    {
        AllLevelsLoaded();
    }
}

void UWorldExecutionManager::AllLevelsLoaded()
{
    bAllLevelsLoaded = true;

    OnAllLevelsLoaded_BP.Broadcast();
    OnAllLevelsLoaded_CPP.Broadcast();

    OnAllLevelsLoaded_BP.Clear();
    OnAllLevelsLoaded_CPP.Clear();

    UE_LOG(LogTemp, Log, TEXT("All levels and nested levels are fully loaded."));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("All levels and nested levels are fully loaded."));
}
