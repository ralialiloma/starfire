#include "Sf_BTService_BlueprintBase.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

void USf_BTService_BlueprintBase::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	
	if (!IsValid(AIOwner->GetPawn()))
	{
		UE_LOG(
			Sf_BTService_BlueprintBase,
			Error,
			TEXT("Owner ist invalid"));
		return;
	}
	OwningAIHolder = Cast<ASf_NPCharacter>(AIOwner->GetPawn());

	if (!IsValid(OwningAIHolder))
	{
		UE_LOG(
			Sf_BTService_BlueprintBase,
			Error,
			TEXT("Require character of type %s but got character of type %s"),
			*ASf_NPCharacter::StaticClass()->GetName(),*NewOwner->GetName());
	}
}

USf_BTService_BlueprintBase::USf_BTService_BlueprintBase(const FObjectInitializer& ObjectInitializer):
	OwningAIHolder(nullptr)
{
}

ASf_NPCharacter* USf_BTService_BlueprintBase::GetOwningNPCharacter()
{
	return OwningAIHolder;
}

FVector USf_BTService_BlueprintBase::GetOwningNPCharacterLocation() const
{
	if (!IsValid(OwningAIHolder))
	{
		UE_LOG(
			Sf_BTService_BlueprintBase,
			Error,
			TEXT("Require character of type %s"),
			*ASf_NPCharacter::StaticClass()->GetName());
		return FVector::Zero();
	}
	return OwningAIHolder->GetActorLocation();
}

UBlackboardComponent* USf_BTService_BlueprintBase::GetOwningBlackboardComponent() const
{
	return UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
}
