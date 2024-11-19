#include "Sf_TetherPointTest.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointGen.h"


TArray<TWeakObjectPtr<UTetherPoint>> USf_TetherPointTest::GetTetherPointsBlocking(
	TWeakObjectPtr<AActor>,
	TWeakObjectPtr<UTetherPoint> CurrentTetherPoint,
	const TArray<TWeakObjectPtr<UTetherPoint>> TetherPoints)
{
	return TetherPoints;
}

void USf_TetherPointTest::FinishExecute()
{
	bFinishedExecute = true;
}
