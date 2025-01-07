#include "Sf_BreakerCore.h"

#include "Kismet/GameplayStatics.h"
#include "Starfire/Shared/Core/Sf_GameState.h"
#include "Starfire/Shared/Damage/Sf_DamageController.h"

DEFINE_LOG_CATEGORY(LogBreakerTarget);

ASf_BreakerCore::ASf_BreakerCore(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ASf_BreakerCore::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->GetBegunPlay())
	{
		StartGame();
	}
	else
	{
		GetWorld()->OnWorldBeginPlay.AddLambda([this]()
		{
			StartGame();
		});
	}
}

void ASf_BreakerCore::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateProgress(ProgressPerSecond * DeltaSeconds);
}

void ASf_BreakerCore::StartGame()
{
	ImportPillars();
	SyncPillars();
	SetActorTickEnabled(true);
}

void ASf_BreakerCore::Stop()
{
	SetActorTickEnabled(false);
}

void ASf_BreakerCore::RegisterPillar(ASf_BreakerPillar* PillarToRegister)
{
	if (!PillarToRegister)
		return;;
	
	if (BreakerPillars.Contains(PillarToRegister))
		return;
	BreakerPillars.AddUnique(PillarToRegister);
	
	PillarToRegister->OnBreak_CPP.AddLambda([this]()
	{
		UpdateProgress(-GetDividedPillarProgress());
	});
}

void ASf_BreakerCore::SyncPillars()
{
	int ActivePillars = GetNumActivePillars();
	int TargetActivePillars = CalculateNumActivePillars();
	int PillarsToSync = TargetActivePillars - ActivePillars;

	for (int i = 0; i < PillarsToSync; ++i)
	{
		GetRandomInactivePillar()->FullRestore();
	}
}

int ASf_BreakerCore::GetNumPillars() const
{
	return BreakerPillars.Num();
}

int ASf_BreakerCore::GetNumActivePillars() const
{
	int NumActivePillars = 0;
	for (auto Pillar : BreakerPillars)
	{
		if (!Pillar)
			continue;

		if (Pillar->IsActive())
			NumActivePillars++;
	}
	return NumActivePillars;
}

float ASf_BreakerCore::GetRawProgress() const
{
	return Progress;
}

float ASf_BreakerCore::GetPillarProgress() const
{
	return Progress - GetNumActivePillars() * GetDividedPillarProgress();
}

void ASf_BreakerCore::OnProgressChanged_Implementation()
{
}

void ASf_BreakerCore::OnProgressFull_Implementation()
{
}

void ASf_BreakerCore::OnProgressEmpty_Implementation()
{
}

void ASf_BreakerCore::UpdateProgress(float Value)
{
	Progress += Value;

	float RestoreProgress = GetPillarProgress();

	while (RestoreProgress > 0)
	{
		if (!ChargingPillar)
		{
			ChargingPillar = GetRandomInactivePillar();
			ChargingDamageHandle = ChargingPillar->DamageController->OnDeathDamage_CPP.AddLambda([this]()
			{
				if (!this)
					return;
				
				float RemainingProgress = 0;
				ChargingPillar->SetRestore(0, RemainingProgress);
				ChargingPillar->BreakPillar();
				ChargingPillar->DamageController->OnDeathDamage_CPP.Remove(ChargingDamageHandle);
				ChargingPillar = nullptr;
				UpdateProgress(-GetPillarProgress());
			});
		}

		if (!ChargingPillar)
			break;

		float RemainingProgress = 0;
		if (ChargingPillar->SetRestore(RestoreProgress, RemainingProgress))
		{
			ChargingPillar->DamageController->OnDeathDamage_CPP.Remove(ChargingDamageHandle);
			ChargingPillar = nullptr;
		}
		
		RestoreProgress = RemainingProgress;
	}

	OnProgressChanged_BP.Broadcast();
	OnProgressChanged_CPP.Broadcast();
	OnProgressChanged();

	//Lose Game
	if (GetNumActivePillars() >= GetNumPillars())
	{
		OnFullProgress_BP.Broadcast();
		OnFullProgress_CPP.Broadcast();
		OnProgressFull();
		Stop();
	}

	//Win Game
	if (GetNumActivePillars() <= 0)
	{
		OnZeroProgress_BP.Broadcast();
		OnZeroProgress_CPP.Broadcast();
		OnProgressEmpty();
		Stop();
	}
}

float ASf_BreakerCore::GetDividedPillarProgress() const
{
	return MaxProgress / GetNumPillars();
}

int ASf_BreakerCore::CalculateNumActivePillars() const
{
	float Value = Progress / GetDividedPillarProgress();
	return FMath::TruncToInt(Value);
}

ASf_BreakerPillar* ASf_BreakerCore::GetRandomInactivePillar()
{
	TArray<ASf_BreakerPillar*> Pillars = GetInactivePillars();
	if (Pillars.IsEmpty())
		return nullptr;

	int Index = FMath::RandRange(0, Pillars.Num() - 1);
	return Pillars[Index];
}

TArray<ASf_BreakerPillar*> ASf_BreakerCore::GetInactivePillars()
{
	TArray<ASf_BreakerPillar*> Pillars = BreakerPillars;

	for (auto Pillar : BreakerPillars)
	{
		if (Pillar && Pillar->IsActive())
			Pillars.Remove(Pillar);
	}

	return Pillars;
}

void ASf_BreakerCore::ImportPillars()
{
	//Import Shards
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this,ASf_BreakerPillar::StaticClass(),FoundActors);
	for (AActor* Actor: FoundActors)
	{
		ASf_BreakerPillar* BreakerShard = Cast<ASf_BreakerPillar>(Actor);
		RegisterPillar(BreakerShard);
	}

	float DividedProgress = GetDividedPillarProgress();
	for (auto Pillar : BreakerPillars)
	{
		Pillar->MaxRestoreValue = DividedProgress;
	}
}