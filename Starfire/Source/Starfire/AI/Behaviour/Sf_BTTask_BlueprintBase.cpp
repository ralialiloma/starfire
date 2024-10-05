#include "Sf_BTTask_BlueprintBase.h"

#include "AIController.h"


USf_BTTask_BlueprintBase::USf_BTTask_BlueprintBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), OwningNPCharacter(nullptr)
{
}

ASf_NPCharacter* USf_BTTask_BlueprintBase::GetOwningSfNPCharacter() const
{
	return OwningNPCharacter;
}

UAnimInstance* USf_BTTask_BlueprintBase::GetOwningAnimInstance() const
{
	if(!IsValid(OwningNPCharacter))
	{
		UE_LOG(
			USf_BTT_BlueprintBase_Log,
			Error,
			TEXT("Invalid character of type %s"),
			*ASf_NPCharacter::StaticClass()->GetName());
		return nullptr;
	}
	return  OwningNPCharacter->GetCharacterAnimInstance();
}

AAIController* USf_BTTask_BlueprintBase::GetOwningAIController() const
{
	if (!OwningNPCharacter) // Check if OwningNPCharacter is valid
	{
		UE_LOG(USf_BTT_BlueprintBase_Log, Warning, TEXT("OwningNPCharacter is null in GetOwningAIController"));
		return nullptr;
	}

	return Cast<AAIController>(OwningNPCharacter->GetController());
}

UEnemyFeature* USf_BTTask_BlueprintBase::GetFeatureByClass(TSubclassOf<UEnemyFeature> Class)
{
	if(!IsValid(OwningNPCharacter))
	{
		UE_LOG(
			USf_BTT_BlueprintBase_Log,
			Error,
			TEXT("Invalid character of type %s"),
			*ASf_NPCharacter::StaticClass()->GetName());
		return nullptr;
	}
	
	return OwningNPCharacter->GetFeatureByClass(Class);
}

void USf_BTTask_BlueprintBase::SetOwner(AActor* NewOwner)
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
			USf_BTT_BlueprintBase_Log,
			Error,
			TEXT("Require character of type %s"),
			*ASf_NPCharacter::StaticClass()->GetName());
	}
	
}


