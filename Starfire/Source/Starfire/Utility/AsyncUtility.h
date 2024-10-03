#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Async/Async.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Math/Vector.h"
#include "Templates/SharedPointer.h"
#include "AsyncUtility.generated.h"


/**
 * A utility class for handling asynchronous operations in Unreal Engine.
 */
USTRUCT()
struct STARFIRE_API FAsyncUtility
{

	GENERATED_BODY();
public:

# pragma region Waiting

	/**
	 * Waits for a future to complete asynchronously on a background thread.
	 * The function periodically checks if the future is ready and only proceeds when it is.
	 * 
	 * @param Future The future to wait for.
	 * @param WorldContextObject The context object to check for validity.
	 * @param CheckInterval The interval, in seconds, to wait between checks. Defaults to 0.01 seconds.
	 */
	template<typename Result>
	static void Wait(
	const TFuture<Result>& Future,
	const TWeakObjectPtr<UObject>& WorldContextObject,
	const float CheckInterval = 0.01f);


	template <typename ResultType>
	static TFuture<void> TestFunction(TArray<TFuture<ResultType>> Futures,TWeakObjectPtr<UObject> WorldContextObject);


	static void Wait(
	const FGraphEventRef GameThreadTask,
	const TWeakObjectPtr<UObject>& WorldContextObject,
	const float CheckInterval = 0.01f);

	/**
	* Waits for all futures in the array to complete asynchronously on a background thread.
	* Collects their results and returns them in a future.
	* 
	* @param Futures The array of futures to wait for.
	* @param WorldContextObject The context object to check for validity.
	* @return A future containing an array of results from the completed futures.
	*/
	template <typename ResultType>
	static TFuture<std::conditional_t<std::is_void_v<ResultType>, void, TArray<ResultType>>> WaitForFutures(
		TArray<TFuture<ResultType>> Futures,
		TWeakObjectPtr<UObject> WorldContextObject);
	
	static void WaitForSeconds(float Duration, const TWeakObjectPtr<UObject>& WorldContextObject, float CheckInterval =0.01f);
	
	/**
	* Blocks the current thread until the next frame is rendered.
	* Useful for synchronization that requires waiting for the next frame.
	*/
	static void WaitOneFrameBlocking(const TWeakObjectPtr<UObject> WorldContextObject);

	/**
	* Executes a function asynchronously that waits for a condition to be met.
	* The condition function will be checked periodically at the specified interval.
	* This function runs on the game thread and does not block background threads.
	* 
	* @param ConditionFunc The function to call that checks the condition.
	* @param WorldContextObject The context object to check for validity.
	* @param CheckInterval The interval, in seconds, to wait between checks. Defaults to 0.01 seconds.
	*/
	static void Wait(
	const TFunction<bool()>& ConditionFunc,
	const TWeakObjectPtr<UObject>& WorldContextObject,
	const float CheckInterval = 0.01f);
	
# pragma endregion Waiting

public:
	/**
	* Streams a level asynchronously and returns a future containing the streaming dynamic level.
	* 
	* @param Name The name of the level instance.
	* @param InteriorLevel The soft object pointer to the world asset.
	* @param Location The location where the level should be spawned.
	* @param WorldContextObject The context object to check for validity.
	* @param bSetInvisible
	* @return A future containing the dynamically streamed level.
	 */
	static TFuture<ULevelStreamingDynamic*> StreamLevel(
		const FString Name,
		const TSoftObjectPtr<UWorld> InteriorLevel,
		const FVector Location,
		const TWeakObjectPtr<UObject> WorldContextObject, bool bSetInvisible =false);
	

	
	
	template <class ThreadSafetyMode,typename... VarTypes>
	static void WaitForEventBlocking(const TWeakObjectPtr<UObject>& WorldContextObject,
	                  TBaseDynamicMulticastDelegate<ThreadSafetyMode,void,VarTypes...>& DelegateToWaitFor,
	                  float CheckInterval = 0.01f);;
	
	template <class ThreadSafetyMode, class ... VarTypes>
	static  TFuture<void> WaitForEvent(
		const TWeakObjectPtr<UObject>& WorldContextObject,
		TBaseDynamicMulticastDelegate<ThreadSafetyMode,void,VarTypes...>& DelegateToWaitFor, float CheckInterval);


	template <class ResultType>
	static  TFuture<ResultType> RunOnGameThread(TUniqueFunction<ResultType()> InFunction);
	
	template <class ResultType>
	static TFuture<ResultType> RunOnAnyThread(TUniqueFunction<ResultType()> InFunction);

private:
	static ULevelStreamingDynamic* ThreadSafeLevelSpawn(
		const FString Name,
		const TSoftObjectPtr<UWorld>& LevelToSpawn,
		const FVector& Location,
		const TWeakObjectPtr<UObject>& WorldContextObject, bool bSetInvisible);


};


template <typename Result>
void FAsyncUtility::Wait(const TFuture<Result>& Future, const TWeakObjectPtr<UObject>& WorldContextObject,
	const float CheckInterval)
{
	while (true)
	{
		if (!WorldContextObject.IsValid())
			return;

		if (Future.IsReady())
			return;
		
		FPlatformProcess::Sleep(CheckInterval);
	}
}

template <typename ResultType>
inline TFuture<void> FAsyncUtility::TestFunction(TArray<TFuture<ResultType>> Futures,TWeakObjectPtr<UObject> WorldContextObject)
{
	TPromise<void> Promise;
	TFuture<void> Future = Promise.GetFuture();
	return Future;
}

template <typename ResultType>
auto FAsyncUtility::WaitForFutures(
	TArray<TFuture<ResultType>> Futures,
	TWeakObjectPtr<UObject> WorldContextObject) 
	-> TFuture<std::conditional_t<std::is_void_v<ResultType>, void, TArray<ResultType>>>
{
	// Handle the case where ResultType is void
	if constexpr (std::is_void_v<ResultType>)
	{
		TPromise<void> Promise;
		TFuture<void> Future = Promise.GetFuture();

		// Async task to wait for all futures to complete
		Async(EAsyncExecution::ThreadPool, [Promise = MoveTemp(Promise), Futures = MoveTemp(Futures)]() mutable {
			for (auto& Future : Futures)
			{
				Future.Wait(); // Wait for each void future
			}
			Promise.SetValue(); // Set promise once all futures complete
		});

		return Future;
	}
	else
	{
		TPromise<TArray<ResultType>> Promise;
		TFuture<TArray<ResultType>> Future = Promise.GetFuture();

		// Async task to wait for all futures and gather results
		Async(EAsyncExecution::ThreadPool, [Promise = MoveTemp(Promise), Futures = MoveTemp(Futures)]() mutable {
			TArray<ResultType> Results;
			Results.Reserve(Futures.Num());

			for (auto& Future : Futures)
			{
				Results.Add(Future.Get()); // Collect results
			}

			Promise.SetValue(MoveTemp(Results)); // Set promise with the results
		});

		return Future;
	}
}

UCLASS()
class UAsyncUtilityEventWaitHelper : public UObject
{
	GENERATED_BODY()

public:
	bool bEventTriggered;

	UAsyncUtilityEventWaitHelper()
		: bEventTriggered(false) {}

	UFUNCTION()
	void OnEventTriggered()
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Event Triggered");
		bEventTriggered = true;
	}
	
};
template <class ThreadSafetyMode, typename ... VarTypes>
void FAsyncUtility::WaitForEventBlocking(const TWeakObjectPtr<UObject>& WorldContextObject,
	TBaseDynamicMulticastDelegate<ThreadSafetyMode, void, VarTypes...>& DelegateToWaitFor, float CheckInterval)
{

	const TFuture<UAsyncUtilityEventWaitHelper*> EventHelperCreation =
		RunOnGameThread<UAsyncUtilityEventWaitHelper*>([&DelegateToWaitFor]() mutable  ->UAsyncUtilityEventWaitHelper* 
		{
			UAsyncUtilityEventWaitHelper* EventWaitHelper = NewObject<UAsyncUtilityEventWaitHelper>();
			EventWaitHelper->bEventTriggered = false;
			TScriptDelegate<ThreadSafetyMode> ScriptDelegate;
			ScriptDelegate.BindUFunction(EventWaitHelper, "OnEventTriggered");
			DelegateToWaitFor.Add(ScriptDelegate);
			return EventWaitHelper;
				
		});

	Wait (EventHelperCreation,WorldContextObject);
	UAsyncUtilityEventWaitHelper* EventWaitHelper = EventHelperCreation.Get();

	// Wait loop
	while (true)
	{
		if (!WorldContextObject.IsValid())
		{
			break;
		}
	
		if (EventWaitHelper->bEventTriggered)
		{
			break;
		}
	
		FPlatformProcess::Sleep(CheckInterval);
	}
	
	// Clean up
	DelegateToWaitFor.Remove(EventWaitHelper,FName("OnEventTriggered"));
}

template <class ThreadSafetyMode, typename ... VarTypes>
TFuture<void> FAsyncUtility::WaitForEvent(const TWeakObjectPtr<UObject>& WorldContextObject,
	TBaseDynamicMulticastDelegate<ThreadSafetyMode,void,VarTypes...> &DelegateToWaitFor, float CheckInterval = 0.01f)
{
	TPromise<void> Promise;
	TFuture<void> Future = Promise.GetFuture();

	FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Promise = MoveTemp(Promise),WorldContextObject,&DelegateToWaitFor,CheckInterval]() mutable->void
		{
			WaitForEventBlocking<ThreadSafetyMode,VarTypes...>(WorldContextObject,DelegateToWaitFor,CheckInterval);
			Promise.SetValue();
			
		}, TStatId(),
		nullptr,
		ENamedThreads::AnyThread);

	return Future;
}



template<typename ResultType>
TFuture<ResultType> FAsyncUtility::RunOnGameThread(TUniqueFunction<ResultType()> InFunction)
{
	TPromise<ResultType> Promise;
	TFuture<ResultType> Future = Promise.GetFuture();
	
	FGraphEventRef GameThreadTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[InFunction = MoveTemp(InFunction), Promise = MoveTemp(Promise)]() mutable 
		{
			if constexpr (std::is_void_v<ResultType>)
			{
				InFunction();
				Promise.SetValue(); 
			}
			else
				Promise.SetValue(InFunction()); 
		},
		TStatId(),
		nullptr,
		ENamedThreads::GameThread);

	return Future;
}


template<typename ResultType>
TFuture<ResultType> FAsyncUtility::RunOnAnyThread(TUniqueFunction<ResultType()> InFunction)
{
	TPromise<ResultType> Promise;
	TFuture<ResultType> Future = Promise.GetFuture();
	
	FGraphEventRef GameThreadTask = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[InFunction = MoveTemp(InFunction), Promise = MoveTemp(Promise)]() mutable 
		{
			if constexpr (std::is_void_v<ResultType>)
			{
				InFunction();
				Promise.SetValue(); 
			}
			else
				Promise.SetValue(InFunction()); 
		},
		TStatId(),
		nullptr,
		ENamedThreads::AnyThread);

	return Future;
}











