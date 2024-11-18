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

	return  CoverGen->LocationInReservedCover(CoverLocation,RadiusToCheck);
}

TArray<UTetherPoint*> USf_TPT_ExcludeReservedLocations::GetTetherPoints(AActor* OwningActor, UTetherPoint* CurrentTetherPoint,
	TArray<UTetherPoint*> TetherPoints)
{
	TArray<UTetherPoint*> ReturnTetherPoints{};
	for (UTetherPoint* TetherPoint: TetherPoints)
	{
		if (!IsLocationReservedTarget(TetherPoint->CenterLocation))
			ReturnTetherPoints.Add(TetherPoint);
	}
	return ReturnTetherPoints;
}
