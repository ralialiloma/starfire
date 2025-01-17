#include "FXHandleObject.h"
#include "FXDataAssetBase.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"

void UFXHandleObject::Cancel()
{
	if (Cancelled)
		return;

	if (RelatedWorld)
	{
		RelatedWorld->GetTimerManager().ClearTimer(TimerHandle);
	}
	
	for (auto FXAssetComponentPairing : FXAssetComponentPairings)
	{
		for (auto SceneComponent : FXAssetComponentPairing.Value.Components)
		{
			if (SceneComponent.IsValid())
			{
				FXAssetComponentPairing.Key->CancelFX(SceneComponent.Get());
			}
		}
	}

	FXAssetComponentPairings.Empty();
	Cancelled = true;
}

bool UFXHandleObject::CanBeCanceled() const
{
	return GetMaxCancelTime() != 0;
}

void UFXHandleObject::OnCompleteData(UWorld* World, FFXHandle Handle)
{
	if (!World)
	{
		return;
	}

	if (CanBeCanceled())
	{
		float CancelTime = GetMaxCancelTime();
		if (CancelTime > 0)
		{
			RelatedWorld = World;
			World->GetTimerManager().SetTimer(TimerHandle, this, &UFXHandleObject::OnTimerCancel, GetMaxCancelTime());
		}
		else if (CancelTime == 0)
		{
			OnTimerCancel();
		}
	}
}

void UFXHandleObject::OnTimerCancel()
{
	UFXSubsystem::Get()->CancelFX(FXHandle);
}

float UFXHandleObject::GetMaxCancelTime() const
{
	float MaxCancelTime = 0;
	for (auto FXAssetComponentPairing : FXAssetComponentPairings)
	{
		for (auto SceneComponent : FXAssetComponentPairing.Value.Components)
		{
			if (SceneComponent.IsValid())
			{
				float CancelTime = FXAssetComponentPairing.Key->GetFXDuration(SceneComponent.Get());
				MaxCancelTime = FMath::Max(MaxCancelTime, CancelTime);
				if (CancelTime == -1)
				{
					//-1 Means Looping
					return -1;
				}
			}
		}
	}
	
	return MaxCancelTime;
}
