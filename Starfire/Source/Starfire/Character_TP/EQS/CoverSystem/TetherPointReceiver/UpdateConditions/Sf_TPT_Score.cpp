#include "Sf_TPT_Score.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"



TArray<UTetherPoint*> USf_TPT_Score::GetTetherPoints(AActor* OwningActor, UTetherPoint* CurrentTetherPoint, TArray<UTetherPoint*> TetherPoints)
{
	TArray<UTetherPoint*> ReturnTetherPoints{};
	for (UTetherPoint* TetherPoint: TetherPoints)
	{
		if (TetherPoint->Score>MinScore &&TetherPoint->Score<MaxScore)
			ReturnTetherPoints.Add(TetherPoint);
	}
	return ReturnTetherPoints;
}
