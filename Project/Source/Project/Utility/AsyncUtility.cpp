#include "AsyncUtility.h"


void FAsyncUtility::WaitForSeconds(float Duration, const TWeakObjectPtr<UObject>& WorldContextObject, float CheckInterval)
{
	const double EndTime = FPlatformTime::Seconds() + Duration;

	while (true)
	{
		// Check if the world context object is still valid
		if (!WorldContextObject.IsValid())
			return;

		// Check if the time has elapsed
		if (FPlatformTime::Seconds() >= EndTime)
			return;

		// Sleep for the specified check interval
		FPlatformProcess::Sleep(CheckInterval);
	}
}


void FAsyncUtility::Wait(const FGraphEventRef GraphEvenRef, const TWeakObjectPtr<UObject>& WorldContextObject,
	const float CheckInterval)
{
	while (true)
	{
		if (!WorldContextObject.IsValid())
			return;

		if (GraphEvenRef->IsComplete())
			return;
		
		FPlatformProcess::Sleep(CheckInterval);
	}
}



ULevelStreamingDynamic* FAsyncUtility::ThreadSafeLevelSpawn(const FString Name,
                                                            const TSoftObjectPtr<UWorld>& LevelToSpawn,
                                                            const FVector& Location,
                                                            const TWeakObjectPtr<UObject>& WorldContextObject,
                                                            bool bSetInvisible)
{
	
	if (LevelToSpawn.IsNull())
	{
		ensureMsgf(false,TEXT( "Level ref invalid"));
		return nullptr;
	}

	if (!WorldContextObject.IsValid())
	{
		ensureMsgf(false,TEXT( "World ContextObj ref invalid"));
		return nullptr;
	}
	
	//Load Level
	bool bLevelLoadSuccess=false;
	ULevelStreamingDynamic* LoadedLevel =
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(WorldContextObject.Get(),
		                                                         LevelToSpawn,
		                                                         Location,
		                                                         FRotator::ZeroRotator,
		                                                         bLevelLoadSuccess,
		                                                         Name);
	if (!bLevelLoadSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Level could not be loaded: %s"), *Name);
		return nullptr;
	}

	LoadedLevel->SetShouldBeVisible(!bSetInvisible);

	return  LoadedLevel;
}

void FAsyncUtility::Wait(const TFunction<bool()>& ConditionFunc, const TWeakObjectPtr<UObject>& WorldContextObject,
	const float CheckInterval)
{
	while (true)
	{
		if (!WorldContextObject.IsValid())
			return;

		if (ConditionFunc())
			return;
		
		FPlatformProcess::Sleep(CheckInterval);
	}
}

void FAsyncUtility::WaitOneFrameBlocking(const TWeakObjectPtr<UObject> WorldContextObject)
{
	checkf(!IsInGameThread(),TEXT("Cannot wait on game thread!"));
	
	const uint32 StartFrameNumber = GFrameCounter;
	
	while (true)
	{
		if (!WorldContextObject.IsValid())
			return;
		
		if (GFrameCounter>StartFrameNumber)
			return;
		
		FPlatformProcess::Sleep(0.001f);
	}
}

TFuture<ULevelStreamingDynamic*> FAsyncUtility::StreamLevel(const FString Name,
	const TSoftObjectPtr<UWorld> InteriorLevel, const FVector Location,
	const TWeakObjectPtr<UObject> WorldContextObject, bool bSetInvisible)
{
	TPromise<ULevelStreamingDynamic*> LevelPromise;
	TFuture<ULevelStreamingDynamic*> LevelFuture = LevelPromise.GetFuture();
	

	//Spawn Level
	FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Name,
			InteriorLevel,
			Location,
			WorldContextObject,
			LevelPromise = MoveTemp(LevelPromise),bSetInvisible]() mutable->void
		{
			ULevelStreamingDynamic* LevelStreamingDynamic =
				ThreadSafeLevelSpawn(Name,InteriorLevel,Location,WorldContextObject, bSetInvisible);

			if (!WorldContextObject.IsValid()||LevelStreamingDynamic==nullptr)
			{
				LevelPromise.SetValue(nullptr);
				return;
			}
			
			AsyncTask(
				ENamedThreads::AnyThread,
				[LevelPromise = MoveTemp(LevelPromise), LevelStreamingDynamic, WorldContextObject,bSetInvisible]() mutable 
				{
				  	
					//Wait Until Level Is Spawned
					FAsyncUtility::Wait ([LevelStreamingDynamic,bSetInvisible]()->bool
					{
						if (LevelStreamingDynamic==nullptr)
							return false;
						
						bool bVisibilityCondition = LevelStreamingDynamic->IsLevelVisible()||bSetInvisible;
						bool bLoaded = LevelStreamingDynamic->IsLevelLoaded();
						bool bUnlocked = !LevelStreamingDynamic->bLocked;

						return bVisibilityCondition && bLoaded &&bUnlocked;
					},WorldContextObject);
					
					LevelPromise.SetValue(LevelStreamingDynamic);

				});
		}, TStatId(),
		nullptr,
		ENamedThreads::GameThread);

	return LevelFuture;
}



