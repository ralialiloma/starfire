#include "Sf_TPT_Score.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"


TArray<TWeakObjectPtr<UTetherPoint>> USf_TPT_Score::GetTetherPointsBlocking(TWeakObjectPtr<AActor>,
                                                                            TWeakObjectPtr<UTetherPoint> CurrentTetherPoint,
                                                                            TArray<TWeakObjectPtr<UTetherPoint>> TetherPoints)
{
	
	TArray<TWeakObjectPtr<UTetherPoint>> ReturnTetherPoints{};
	for (TWeakObjectPtr<UTetherPoint> TetherPoint: TetherPoints)
	{
		if (TetherPoint->CoverPotential>MinScore && TetherPoint->CoverPotential<MaxScore)
		{
			ReturnTetherPoints.Add(TetherPoint);
			//FString DebugString = FString::Printf(TEXT("CoverPotential: %f with Min %f and Max %f"),TetherPoint->CoverPotential,MinScore,MaxScore);
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, DebugString);
		}
			
	}
	return ReturnTetherPoints;
}
