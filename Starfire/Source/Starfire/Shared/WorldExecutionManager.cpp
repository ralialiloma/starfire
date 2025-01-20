#include "WorldExecutionManager.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "LevelInstance/LevelInstanceActor.h"

DEFINE_LOG_CATEGORY(LogWorldExecutionManager)

void UWorldExecutionManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    for (auto Actor : GetWorld()->GetCurrentLevel()->Actors)
    {
        if (Actor.IsA<ALevelInstance>())
        {
            NumLevelsFound++;
        }
    }
    
    FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UWorldExecutionManager::OnLevelAddedToWorld);

    CheckAllLevelsLoaded();
}

void UWorldExecutionManager::Deinitialize()
{
    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
    FWorldDelegates::LevelAddedToWorld.RemoveAll(this);
    
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
        Callback();
    }
    else
    {
        OnAllLevelsLoaded_CPP.AddLambda(Callback);
    }
}

void UWorldExecutionManager::OnLevelAddedToWorld(ULevel* Level, UWorld* World)
{
    if (World == GetWorld() && Level)
    {
        FString LevelName = Level->GetOuter()->GetName();
        UE_LOG(LogWorldExecutionManager, Log, TEXT("Level added to world: %s"), *LevelName);
        NumLevelsFound--;
        
        TrackNestedLevels(World);
        CheckAllLevelsLoaded();
    }
}

void UWorldExecutionManager::TrackNestedLevels(UWorld* World)
{
    if (!World)
    {
        return;
    }

    bool FoundStreamingLevels = false;

    for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
    {
        if (StreamingLevel)
        {
            FoundStreamingLevels = true;
            FString StreamingLevelName = StreamingLevel->GetWorldAssetPackageName();
            
            if (!StreamingLevel->HasLoadedLevel())
            {
                NumLevelsLoading++;
                StreamingLevel->OnLevelShown.AddDynamic(this, &UWorldExecutionManager::OnLevelShown);

                UE_LOG(LogWorldExecutionManager, Log, TEXT("Tracking nested level: %s"), *StreamingLevelName);
            }
        }
    }

    if (!FoundStreamingLevels && NumLevelsLoading == 0)
    {
        UE_LOG(LogWorldExecutionManager, Log, TEXT("No streaming levels found. Treating as fully loaded."));
    }
}

void UWorldExecutionManager::OnLevelShown()
{
    NumLevelsLoading--;

    UE_LOG(LogWorldExecutionManager, Log, TEXT("A level finished loading. Remaining: %d"), NumLevelsLoading);

    CheckAllLevelsLoaded();
}

void UWorldExecutionManager::CheckAllLevelsLoaded()
{
    if (NumLevelsLoading <= 0 && NumLevelsFound <= 0 && !bAllLevelsLoaded)
    {
        AllLevelsLoaded();
    }
}

void UWorldExecutionManager::AllLevelsLoaded()
{
    bAllLevelsLoaded = true;

    OnAllLevelsLoaded_BP.Broadcast();
    OnAllLevelsLoaded_CPP.Broadcast();

    UE_LOG(LogWorldExecutionManager, Log, TEXT("All levels and nested levels are fully loaded."));
}
