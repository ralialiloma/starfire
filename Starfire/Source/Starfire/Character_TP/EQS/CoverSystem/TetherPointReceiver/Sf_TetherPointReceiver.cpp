#include "Sf_TetherPointReceiver.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Starfire/Character_TP/EQS/CoverSystem/Sf_TetherPointSubsystem.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"

void USf_TetherPointReceiver::BeginPlay()
{
	Super::BeginPlay();

	for (USf_TetherPointTest* Test: Tests)
		WeakTests.Add(Test);
	RunUpdateLoop();
}


TFuture<void> USf_TetherPointReceiver::Update()
{
	TWeakObjectPtr<USf_TetherPointReceiver> WeakSelf = this;
	return FAsyncUtility::RunOnAnyThread<void>(WeakSelf,[WeakSelf]()->void
		{
			TFuture<TArray<TWeakObjectPtr<UTetherPoint>>> ImportTetherPoints = WeakSelf->ImportTetherPoints();
			FAsyncUtility::Wait(ImportTetherPoints,WeakSelf);
			TArray<TWeakObjectPtr<UTetherPoint>> WeakTetherPoints =
				FAsyncUtility::GetFutureValue<TArray<TWeakObjectPtr<UTetherPoint>>>(MoveTemp(ImportTetherPoints));
		
			//Test Thread
			const TFuture<void> TestThread =
				FAsyncUtility::RunOnAnyThread<void>(WeakSelf,[WeakSelf,WeakTetherPoints]()->void
				{
					//Runt Tests Async
					TFuture<TArray<TWeakObjectPtr<UTetherPoint>>> TetherPointFuture = WeakSelf->RunTests_Async(WeakTetherPoints);
					FAsyncUtility::Wait(TetherPointFuture,WeakSelf,0.1f,5);
					TArray<TWeakObjectPtr<UTetherPoint>> TestedPoints =
						FAsyncUtility::GetFutureValue<TArray<TWeakObjectPtr<UTetherPoint>>>(MoveTemp(TetherPointFuture));
					
					//Assign Point on Gamethread
					FAsyncUtility::RunOnGameThread<void>(WeakSelf,[WeakSelf,TestedPoints]()->void
					{
						if (TestedPoints.Num()>0)
						{
							WeakSelf->UpdateTetherPoint(TestedPoints[0].Get());
						}
					});
				}
				);
			FAsyncUtility::Wait(TestThread, WeakSelf);
		});
}

TFuture<TArray<TWeakObjectPtr<UTetherPoint>>> USf_TetherPointReceiver::ImportTetherPoints()
{
	TWeakObjectPtr<USf_TetherPointReceiver> WeakSelf = this;
	return FAsyncUtility::RunOnGameThread<TArray<TWeakObjectPtr<UTetherPoint>>>(WeakSelf,[WeakSelf]()->TArray<TWeakObjectPtr<UTetherPoint>>
		{
			TArray<TWeakObjectPtr<UTetherPoint>> WeakTetherPoints{};
				if (!WeakSelf.IsValid())
					return WeakTetherPoints;
				if (!WeakSelf->NeedsUpdate())
					return WeakTetherPoints;
				const UWorld* World = WeakSelf->GetWorld();
				if (!IsValid(World))
					return WeakTetherPoints;
				const UGameInstance* GameInstance = WeakSelf->GetWorld()->GetGameInstance();
				if (!IsValid(GameInstance))
					return WeakTetherPoints;
				ASf_TetherPointGen* TetherPointGen  = GameInstance->GetSubsystem<USf_TetherPointSubsystem>()->GetTetherPointGen();
				if (!IsValid(TetherPointGen))
					return WeakTetherPoints;
					
				const TArray<UTetherPoint*> AllTetherPoints =  TetherPointGen->GetRelevantTetherPoints();
				WeakTetherPoints.Reserve(AllTetherPoints.Num());
				for (UTetherPoint* TetherPoint : AllTetherPoints)
					WeakTetherPoints.Add(TetherPoint);
		
				return WeakTetherPoints;
		});

}

TFuture<TArray<TWeakObjectPtr<UTetherPoint>>> USf_TetherPointReceiver::RunTests_Async(const TArray<TWeakObjectPtr<UTetherPoint>>& AllTetherPoints)
{

	TWeakObjectPtr<AActor> Owner = GetOwner();
	TWeakObjectPtr<USf_TetherPointReceiver> WeakSelf = this;
	return  FAsyncUtility::RunOnAnyThread<TArray<TWeakObjectPtr<UTetherPoint>>>(
		WeakSelf,[AllTetherPoints,Owner,WeakSelf]()->TArray<TWeakObjectPtr<UTetherPoint>>
		{
			TArray<TWeakObjectPtr<UTetherPoint>> Points = AllTetherPoints;
			if (!WeakSelf.IsValid())
				return Points;

			TWeakObjectPtr<UTetherPoint> WeakActiveTetherPoint = WeakSelf->ActiveTetherPoint;
			for (TWeakObjectPtr<USf_TetherPointTest> Test: WeakSelf->WeakTests)
			{
				if (!WeakSelf.IsValid())
					return Points;

				if (!Test.IsValid())
					return Points;
				
				Points =  Test->GetTetherPointsBlocking(Owner,WeakActiveTetherPoint, Points);
				
				if (Points.Num()<=0)
				{
					return Points;
				}
			}
			return Points;
		});
}



UTetherPoint* USf_TetherPointReceiver::GetActiveTetherPoint() const
{
	return ActiveTetherPoint;
}

void USf_TetherPointReceiver::RunUpdateLoop()
{
	TWeakObjectPtr<USf_TetherPointReceiver> WeakSelf = this;
	FAsyncUtility::RunOnAnyThread<void>(WeakSelf,[WeakSelf]()->void
		{
			while (true)
			{
				FAsyncUtility::Wait(WeakSelf->Update(),WeakSelf,0.2f,10);
				FAsyncUtility::WaitForSeconds(1,WeakSelf);
			}
		});
}

bool USf_TetherPointReceiver::NeedsUpdate()
{
	/*TArray<UTetherPoint*> ResultPoints =  RunTests(TArray<UTetherPoint*>{ActiveTetherPoint});
	if (ResultPoints.Contains(ActiveTetherPoint))
		return false;
	return true;*/
	return true;
}

void USf_TetherPointReceiver::UpdateTetherPoint(UTetherPoint* NewTetherPoint)
{
	
	ActiveTetherPoint = NewTetherPoint;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!IsValid(Character))
		return;
	AAIController* Controller =   Cast<AAIController>(Character->GetController());
	if (!IsValid(Controller))
		return;
	UBlackboardComponent* BlackboardComponent =  Controller->GetBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,LocationToUpdate,ActiveTetherPoint->CenterLocation);


	UKismetSystemLibrary::DrawDebugArrow(this,GetOwner()->GetActorLocation(),ActiveTetherPoint->CenterLocation,20,FColor::Red,5,10);
	
	UDebugFunctionLibrary::Sf_DrawDebugSphere(
		this,
		Sf_GameplayTags::Debug::TP::EQS::PeakLocation,
		ActiveTetherPoint->CenterLocation,
		150,
		6,
		FColor::Purple,
		3,
		10);
}
