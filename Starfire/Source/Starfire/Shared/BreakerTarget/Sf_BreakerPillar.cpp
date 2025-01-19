#include "Sf_BreakerPillar.h"

#include "Starfire/Shared/ActionLogger/ActionLogger.h"
#include "Starfire/Shared/Core/Sf_GameState.h"
#include "Starfire/Shared/Sound/FXSubsystem.h"
#include "Templates/Function.h"

ASf_BreakerPillar::ASf_BreakerPillar()
{
	//Damage Controller
	DamageController = CreateDefaultSubobject<USf_DamageController>(TEXT("DamageController"));
	DamageController->bEnablePassiveHealing = false;
	DamageController->SupportedDamageTypes = FGameplayTagContainer(Sf_GameplayTags::Gameplay::DamageType::Explosion);
	DamageController->bStartWithMaxHealth = false;
	DamageController->MaxHealth = 1;

	//Hitbox
	PrimaryHitbox = CreateDefaultSubobject<USf_Hitbox>(TEXT("Hitbox"));
	PrimaryHitbox->SetUsingAbsoluteLocation(false);
	PrimaryHitbox->SetUsingAbsoluteRotation(false);
	PrimaryHitbox->SetUsingAbsoluteScale(false);

	RootComponent = PrimaryHitbox;
}

void ASf_BreakerPillar::BeginPlay()
{
	Super::BeginPlay();

	if (bIndependent)
		FullRestore();

	DamageController->OnZeroHealth_CPP.AddLambda([this]()
	{
		BreakPillar();
	});
}

USf_DamageController* ASf_BreakerPillar::GetDamageController() const
{
	return DamageController;
}

bool ASf_BreakerPillar::IsActive() const
{
	if (IsValid(DamageController))
		return !DamageController->IsZeroHealth();
	return false;
}

bool ASf_BreakerPillar::SetRestore(float RestoreValue, float& OutRemainingValue)
{
	if (!IsValid(this))
		return false;
	
	OutRemainingValue = RestoreValue;
	
	if (IsActive())
		return false;

	CurrentRestoreValue = FMath::Min(RestoreValue, MaxRestoreValue);
	OutRemainingValue = FMath::Max(RestoreValue - MaxRestoreValue, 0.0f);

	OnRestoreProgress(FMath::Clamp(CurrentRestoreValue / MaxRestoreValue, 0.f, 1.f));

	if (OutRemainingValue > 0.0f)
		FullRestore();

	const FActionLog Log = FActionLog (Sf_GameplayTags::Gameplay::ActionLogger::PortalSystem::Pillar::RestoreProgress,GetActorLocation());

	ASf_GameState* GameState = GetWorld()->GetGameState<ASf_GameState>();
	if (GameState && GameState->GetPlayState() == Sf_GameplayTags::Gameplay::PlayState::Arena)
	{
		UActionLoggerSubSystem::Get(GetWorld())->ReportAction(Log);
		UFXSubsystem::Get()->PlayFX(this, Sf_GameplayTags::Effects::Messages::PortalSystem::Pillar::RestoreProgress);
	}
	
	return OutRemainingValue > 0.0f;
}

void ASf_BreakerPillar::FullRestore()
{
	if (IsActive())
		return;

	CurrentRestoreValue = 0;
	DamageController->Reset();
	DamageController->SetHealth(1);
	ASf_GameState* GameState = GetWorld()->GetGameState<ASf_GameState>();
	if (GameState && GameState->GetPlayState() == Sf_GameplayTags::Gameplay::PlayState::Arena)
	{
		const FActionLog Log = FActionLog (Sf_GameplayTags::Gameplay::ActionLogger::PortalSystem::Pillar::Restore,GetActorLocation());
		UActionLoggerSubSystem::Get(GetWorld())->ReportAction(Log);
		UFXSubsystem::Get()->PlayFX(this, Sf_GameplayTags::Effects::Messages::PortalSystem::Pillar::Restore);
	}
	OnRestore();
}

void ASf_BreakerPillar::BreakPillar()
{
	OnBreak();
	OnBreak_BP.Broadcast();
	OnBreak_CPP.Broadcast();
	const FActionLog Log = FActionLog (Sf_GameplayTags::Gameplay::ActionLogger::PortalSystem::Pillar::Break,GetActorLocation());
	UActionLoggerSubSystem::Get(GetWorld())->ReportAction(Log);
	UFXSubsystem::Get()->PlayFX(this, Sf_GameplayTags::Effects::Messages::PortalSystem::Pillar::Break);
}

void ASf_BreakerPillar::OnBreak_Implementation()
{

}

void ASf_BreakerPillar::OnRestore_Implementation()
{
}

void ASf_BreakerPillar::OnRestoreProgress_Implementation(float Alpha)
{
}