
#include "Sf_TetherPointSubsystem.h"

#include "Sf_TetherPointGen.h"
#include "Kismet/GameplayStatics.h"




void USf_TetherPointSubsystem::RegisterTetherPointGen(ASf_TetherPointGen* ActorToRegister)
{
	TetherPointGen = ActorToRegister;
}

ASf_TetherPointGen* USf_TetherPointSubsystem::GetTetherPointGen() const
{
	if (!IsValid(TetherPointGen))
	{
		UE_LOG(LogTemp, Error, TEXT("No Tether Point Gen found!"))
		return nullptr;
	}
	return TetherPointGen;
}

USf_TetherPointSubsystem* USf_TetherPointSubsystem::Get(const UWorld* World)
{
	if (IsValid(World))
		return  World->GetGameInstance()->GetSubsystem<USf_TetherPointSubsystem>();
	return 	nullptr;
}
