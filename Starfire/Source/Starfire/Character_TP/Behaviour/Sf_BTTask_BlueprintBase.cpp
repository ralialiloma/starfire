#include "Sf_BTTask_BlueprintBase.h"

#include "AIController.h"
#include "BlackboardKeyHelperLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


USf_BTTask_BlueprintBase::USf_BTTask_BlueprintBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), OwningNPCharacter(nullptr)
{
}

ASf_TP_Character* USf_BTTask_BlueprintBase::GetOwningSfNPCharacter() const
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
			*ASf_TP_Character::StaticClass()->GetName());
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

UBlackboardComponent* USf_BTTask_BlueprintBase::GetOwningBlackboard() const
{
	return GetOwningAIController()->GetBlackboardComponent();
}

USf_CharacterFeature* USf_BTTask_BlueprintBase::GetFeatureByClass(TSubclassOf<USf_CharacterFeature> Class)
{
	if(!IsValid(OwningNPCharacter))
	{
		UE_LOG(
			USf_BTT_BlueprintBase_Log,
			Error,
			TEXT("Invalid character of type %s"),
			*ASf_TP_Character::StaticClass()->GetName());
		return nullptr;
	}
	
	return OwningNPCharacter->GetFeatureByClass(Class);
}

void USf_BTTask_BlueprintBase::Sf_SetBlackboardFloatValue(const EFloatBlackboardKey FloatBlackboardKey, const float Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());
	UBlackboardKeyHelperLibrary::SetFloatValue(BlackboardComponent,FloatBlackboardKey,Value);
}

void USf_BTTask_BlueprintBase::Sf_SetBlackboardIntValue(const EIntBlackboardKey IntBlackboardKey, const int32 Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());
	UBlackboardKeyHelperLibrary::SetIntValue(BlackboardComponent,IntBlackboardKey,Value);
}

void USf_BTTask_BlueprintBase::Sf_SetBlackboardBoolValue(const EBoolBlackboardKey BoolBlackboardKey, const bool Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,BoolBlackboardKey,Value);
}

void USf_BTTask_BlueprintBase::Sf_SetBlackboardVectorValue(const ELocationBlackboardKey ActorBlackboardKey, const FVector Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ActorBlackboardKey,Value);
}


void USf_BTTask_BlueprintBase::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);
	
	if (!IsValid(AIOwner->GetPawn()))
	{
		UE_LOG(
			USf_BTT_BlueprintBase_Log,
			Error,
			TEXT("Owner ist invalid"));
		return;
	}
	OwningNPCharacter = Cast<ASf_TP_Character>(AIOwner->GetPawn());

	if (!IsValid(OwningNPCharacter))
	{
		UE_LOG(
			USf_BTT_BlueprintBase_Log,
			Error,
			TEXT("Require character of type %s but got character of type %s"),
			*ASf_TP_Character::StaticClass()->GetName(),*NewOwner->GetName());
	}
	
}


