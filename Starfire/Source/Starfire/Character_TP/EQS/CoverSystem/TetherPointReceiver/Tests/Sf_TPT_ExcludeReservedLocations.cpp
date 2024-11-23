#include "Sf_TPT_ExcludeReservedLocations.h"

#include "Kismet/GameplayStatics.h"
#include "Starfire/Character_TP/EQS/NavigationTargetSubsystem.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"



bool USf_TPT_ExcludeReservedLocations::IsLocationReservedTarget(const FVector& CoverLocation) const
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (GameInstance==nullptr)
		return true;
	const auto CoverGen =  GameInstance->GetSubsystem<UNavigationTargetSubsystem>();

	if (CoverGen==nullptr)
		return true;

	return  CoverGen->HasCloseNavTarget(CoverLocation,RadiusToCheck);
}

TArray<TWeakObjectPtr<UTetherPoint>> USf_TPT_ExcludeReservedLocations::GetTetherPointsBlocking(TWeakObjectPtr<AActor>,
                                                                                               TWeakObjectPtr<UTetherPoint> CurrentTetherPoint,
                                                                                               TArray<TWeakObjectPtr<UTetherPoint>> TetherPoints)
{
	TArray<TWeakObjectPtr<UTetherPoint>> ReturnTetherPoints{};
	for (TWeakObjectPtr<UTetherPoint> TetherPoint: TetherPoints)
	{
		if (!IsLocationReservedTarget(TetherPoint->CenterLocation))
			ReturnTetherPoints.Add(TetherPoint);
	}
	return ReturnTetherPoints;
}
