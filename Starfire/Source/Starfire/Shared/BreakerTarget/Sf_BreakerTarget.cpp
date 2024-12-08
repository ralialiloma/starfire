#include "Sf_BreakerTarget.h"

#include "Kismet/GameplayStatics.h"
#include "Starfire/Shared/Core/Sf_GameState.h"
#include "Starfire/Shared/Damage/Sf_DamageController.h"

DEFINE_LOG_CATEGORY(LogBreakerTarget);

ASf_BreakerTarget::ASf_BreakerTarget(const FObjectInitializer& ObjectInitializer): ProgressionRatePerSecond(0.05)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickInterval = 1.0f;
	
	//Damage Controller
	DamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("DamageController"));
	DamageController->bEnablePassiveHealing = false;
	DamageController->bStartWithMaxHealth = false;
}

void ASf_BreakerTarget::BeginPlay()
{
	Super::BeginPlay();
	
	GameState = GetWorld()->GetGameState<ASf_GameState>();

	TWeakObjectPtr<ASf_BreakerTarget> WeakSelf = this;
	if (!IsValid(DamageController))
	{
		UE_LOG(LogBreakerTarget, Error, TEXT("Invalid %s"),*USf_DamageController::StaticClass()->GetName())
		return;
	}
	
	DelHandle =  DamageController->OnHealthChanged_CPP.AddLambda([WeakSelf]()->void
	{
		if (!WeakSelf.IsValid())
		{
			return;
		}
		
		WeakSelf->OnProgressChanged_CPP.Broadcast();
		WeakSelf->OnProgressChanged_BP.Broadcast();
	});

	DamageController->OnFullHealth_CPP.AddLambda([WeakSelf]()->void
	{
		if (!WeakSelf.IsValid())
		{
			return;
		}
		
		WeakSelf->OnFullProgress_CPP.Broadcast();
		WeakSelf->OnFullProgress_BP.Broadcast();

		if (WeakSelf.IsValid() && WeakSelf.Get()->GameState)
		{
			WeakSelf.Get()->GameState->EndGame(false);
		}
	});

	DamageController->OnZeroHealth_CPP.AddLambda([WeakSelf]()->void
	{
		if (!WeakSelf.IsValid())
		{
			return;
		}
		
		WeakSelf->OnZeroProgress_BP.Broadcast();
		WeakSelf->OnZeroProgress_CPP.Broadcast();

		if (WeakSelf.IsValid() && WeakSelf.Get()->GameState)
		{
			WeakSelf.Get()->GameState->EndGame(true);
		}
	});
}

void ASf_BreakerTarget::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GameState && GameState->IsInPlayState(Sf_GameplayTags::Gameplay::PlayState::Arena))
	{
		//DoProgress(DeltaSeconds);
	}
}

void ASf_BreakerTarget::RegisterShard(ASf_BreakerShard* ShardToRegister)
{
	if (!IsValid(ShardToRegister))
		return;;
	if(BreakerShards.Contains(ShardToRegister))
		return;
	BreakerShards.AddUnique(ShardToRegister);
	ShardToRegister->GetDamageController()->OnHealthChanged_CPP.AddLambda([this]()->void{UpdateTotalHealth();});

	//Get all shard tags
	FGameplayTagContainer ShardContainer = ShardToRegister->GetDamageController()->SupportedDamageTypes;
	for (const FGameplayTag& CurrentTag : ShardContainer)
	{
		if (!DamageController->SupportedDamageTypes.HasTag(CurrentTag))
			DamageController->SupportedDamageTypes.AddTag(CurrentTag);
	}
	UpdateTotalHealth();
}

void ASf_BreakerTarget::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetWorld() && GetWorld()->GetGameInstance())
		ImportShards();
}



float ASf_BreakerTarget::GetProgress() const
{
	return DamageController->GetCurrentHealthInPercent();
}

USf_DamageController* ASf_BreakerTarget::GetDamageController() const
{
	return DamageController;
}

void ASf_BreakerTarget::UpdateTotalHealth()
{
	float CurrentHealth = 0;
	float MaxHealth = 0;
	for (const ASf_BreakerShard* Shard: BreakerShards)
	{
		if (!IsValid(Shard))
			continue;
		const USf_DamageController* CurrentController = Shard->GetDamageController();
		float CurrentShardHealth =  CurrentController->GetCurrentHealth();
		float ShardMaxHealth = CurrentController->MaxHealth;

		CurrentHealth+=CurrentShardHealth;
		MaxHealth += ShardMaxHealth;
	}
	DamageController->MaxHealth = MaxHealth;
	DamageController->SetHealth(CurrentHealth);
}

void ASf_BreakerTarget::ImportShards()
{
	//Import Shards
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this,ASf_BreakerShard::StaticClass(),FoundActors);
	for (AActor* Actor: FoundActors)
	{
		ASf_BreakerShard* BreakerShard = Cast<ASf_BreakerShard>(Actor);
		RegisterShard(BreakerShard);
	}
}



