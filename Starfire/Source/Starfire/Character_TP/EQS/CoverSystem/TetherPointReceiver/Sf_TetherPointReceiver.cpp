#include "Sf_TetherPointReceiver.h"

#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointSubsystem.h"

void USf_TetherPointReceiver::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(UpdateProcess,[this]()->void{Update();},UpdateRateInSeconds,true);
}



void USf_TetherPointReceiver::Update()
{
	if (!NeedsUpdate())
		return;
	
	const UWorld* World = GetWorld();
	if (!IsValid(World))
		return;
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!IsValid(GameInstance))
		return;
	ASf_TetherPointGen* TetherPointGen  = GameInstance->GetSubsystem<USf_TetherPointSubsystem>()->GetTetherPointGen();
	if (!IsValid(TetherPointGen))
		return;
	
	const TArray<UTetherPoint*> AllTetherPoints =  TetherPointGen->GetAllTetherPoints();
	TArray<UTetherPoint*> NewTetherPoints = RunTests(AllTetherPoints);
	
	if (NewTetherPoints.Num()>0)
		ActiveTetherPoint = NewTetherPoints[0];
}

TArray<UTetherPoint*> USf_TetherPointReceiver::RunTests(const TArray<UTetherPoint*>& AllTetherPoints)
{
	TArray<UTetherPoint*> Points = AllTetherPoints;
	for (USf_TetherPointTest* Test: Tests)
	{
		Points =  Test->GetTetherPoints(GetOwner(),ActiveTetherPoint, Points);
		if (Points.Num()<=0)
		{
			return Points;
		}
	}
	return Points;
}



UTetherPoint* USf_TetherPointReceiver::GetActiveTetherPoint() const
{
	return ActiveTetherPoint;
}

bool USf_TetherPointReceiver::NeedsUpdate()
{
	TArray<UTetherPoint*> ResultPoints =  RunTests(TArray<UTetherPoint*>{ActiveTetherPoint});
	if (ResultPoints.Contains(ActiveTetherPoint))
		return false;
	return true;
}
