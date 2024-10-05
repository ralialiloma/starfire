#include "Sf_BTService_BlueprintBase.h"

void USf_BTService_BlueprintBase::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	if (!IsValid(NewOwner))
	{
		return;
	}
	OwningNPCharacter = Cast<ASf_NPCharacter>(NewOwner);

	if (IsValid(OwningNPCharacter))
	{
		UE_LOG(
			Sf_BTService_BlueprintBase_Log,
			Error,
			TEXT("Require character of type %s"),
			*ASf_NPCharacter::StaticClass()->GetName());
	}
}

USf_BTService_BlueprintBase::USf_BTService_BlueprintBase(const FObjectInitializer& ObjectInitializer):
	OwningNPCharacter(nullptr)
{
}

ASf_NPCharacter* USf_BTService_BlueprintBase::GetOwningNPCharacter()
{
	return OwningNPCharacter;
}

FVector USf_BTService_BlueprintBase::GetOwningNPCharacterLocation() const
{
	if (IsValid(OwningNPCharacter))
	{
		UE_LOG(
			Sf_BTService_BlueprintBase_Log,
			Error,
			TEXT("Require character of type %s"),
			*ASf_NPCharacter::StaticClass()->GetName());
		return FVector::Zero();
	}
	return OwningNPCharacter->GetActorLocation();
}
