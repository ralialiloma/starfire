#include "Sf_BTService_BlueprintBase.h"

#include "BlackboardKeyHelperLibrary.h"
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
	OwningAIHolder = Cast<ASf_TP_Character>(AIOwner->GetPawn());

	if (!IsValid(OwningAIHolder))
	{
		UE_LOG(
			Sf_BTService_BlueprintBase,
			Error,
			TEXT("Require character of type %s but got character of type %s"),
			*ASf_TP_Character::StaticClass()->GetName(),*NewOwner->GetName());
	}
}

USf_BTService_BlueprintBase::USf_BTService_BlueprintBase(const FObjectInitializer& ObjectInitializer):
	OwningAIHolder(nullptr)
{
}

ASf_TP_Character* USf_BTService_BlueprintBase::GetOwningNPCharacter()
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
			*ASf_TP_Character::StaticClass()->GetName());
		return FVector::Zero();
	}
	return OwningAIHolder->GetActorLocation();
}

UBlackboardComponent* USf_BTService_BlueprintBase::GetOwningBlackboardComponent() const
{
	return UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
}

void USf_BTService_BlueprintBase::Sf_SetBlackboardFloatValue(const EFloatBlackboardKey FloatBlackboardKey, const float Value)
{
	UBlackboardComponent* BlackboardComponent = GetOwningBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetFloatValue(BlackboardComponent,FloatBlackboardKey,Value);
}

void USf_BTService_BlueprintBase::Sf_SetBlackboardIntValue(const EIntBlackboardKey IntBlackboardKey, const int32 Value)
{
	UBlackboardComponent* BlackboardComponent = GetOwningBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetIntValue(BlackboardComponent,IntBlackboardKey,Value);
}

void USf_BTService_BlueprintBase::Sf_SetBlackboardBoolValue(const EBoolBlackboardKey BoolBlackboardKey, const bool Value)
{
	UBlackboardComponent* BlackboardComponent = GetOwningBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,BoolBlackboardKey,Value);
}

void USf_BTService_BlueprintBase::Sf_SetBlackboardVectorValue(const ELocationBlackboardKey ActorBlackboardKey, const FVector Value)
{
	UBlackboardComponent* BlackboardComponent = GetOwningBlackboardComponent();
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ActorBlackboardKey,Value);
}
