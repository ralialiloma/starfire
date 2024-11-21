#include "Sf_BTDecorator_BlueprintBase.h"
#include "Sf_BTTask_BlueprintBase.h"
#include "AIController.h"
#include "BlackboardKeyHelperLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"


USf_BTDecorator_BlueprintBase::USf_BTDecorator_BlueprintBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), OwningNPCharacter(nullptr)
{
}

ASf_TP_Character* USf_BTDecorator_BlueprintBase::GetOwningSfNPCharacter() const
{
	return OwningNPCharacter;
}

UAnimInstance* USf_BTDecorator_BlueprintBase::GetOwningAnimInstance() const
{
	if(!IsValid(OwningNPCharacter))
	{
		UE_LOG(
			USf_BTDecorator_BlueprintBase_Log,
			Error,
			TEXT("Invalid character of type %s"),
			*ASf_TP_Character::StaticClass()->GetName());
		return nullptr;
	}
	return  OwningNPCharacter->GetCharacterAnimInstance();
}

AAIController* USf_BTDecorator_BlueprintBase::GetOwningAIController() const
{
	if (!OwningNPCharacter) // Check if OwningNPCharacter is valid
	{
		UE_LOG(USf_BTDecorator_BlueprintBase_Log, Warning, TEXT("OwningNPCharacter is null in GetOwningAIController"));
		return nullptr;
	}

	return Cast<AAIController>(OwningNPCharacter->GetController());
}

UBlackboardComponent* USf_BTDecorator_BlueprintBase::GetOwningBlackboard() const
{
	return GetOwningAIController()->GetBlackboardComponent();
}

USf_CharacterFeature* USf_BTDecorator_BlueprintBase::GetFeatureByClass(TSubclassOf<USf_CharacterFeature> Class)
{
	if(!IsValid(OwningNPCharacter))
	{
		UE_LOG(
			USf_BTDecorator_BlueprintBase_Log,
			Error,
			TEXT("Invalid character of type %s"),
			*ASf_TP_Character::StaticClass()->GetName());
		return nullptr;
	}
	
	return OwningNPCharacter->GetFeatureByClass(Class);
}

void USf_BTDecorator_BlueprintBase::Sf_SetBlackboardFloatValue(const EFloatBlackboardKey FloatBlackboardKey, const float Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());
	UBlackboardKeyHelperLibrary::SetFloatValue(BlackboardComponent,FloatBlackboardKey,Value);
}

void USf_BTDecorator_BlueprintBase::Sf_SetBlackboardIntValue(const EIntBlackboardKey IntBlackboardKey, const int32 Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());
	UBlackboardKeyHelperLibrary::SetIntValue(BlackboardComponent,IntBlackboardKey,Value);
}

void USf_BTDecorator_BlueprintBase::Sf_SetBlackboardBoolValue(const EBoolBlackboardKey BoolBlackboardKey, const bool Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,BoolBlackboardKey,Value);
}

void USf_BTDecorator_BlueprintBase::Sf_SetBlackboardVectorValue(const ELocationBlackboardKey ActorBlackboardKey, const FVector Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ActorBlackboardKey,Value);
}

bool USf_BTDecorator_BlueprintBase::Sf_IsVectorKeyValidAndSet(ELocationBlackboardKey LocationBlackboardKey) const
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(GetOwningAIController());

	if (!UBlackboardKeyHelperLibrary::VerifyVectorValue(BlackboardComponent,LocationBlackboardKey))
		return false;

	if (!UBlackboardKeyHelperLibrary::IsSetVectorValue(BlackboardComponent,LocationBlackboardKey))
		return false;
	
	return  true;
}

void USf_BTDecorator_BlueprintBase::Sf_ThrowInvalidLocationKeyError(ELocationBlackboardKey InvalidKey) const
{
	FString InvalidKeyName = *USf_FunctionLibrary::GetEnumAsString<ELocationBlackboardKey>(InvalidKey);
	FString ErrorMessage = FString::Printf(TEXT("Invalid or not set blackboard key for (%s) "),*InvalidKeyName);
	UDebugFunctionLibrary::Sf_ThrowError(this,ErrorMessage,true,true);
}


void USf_BTDecorator_BlueprintBase::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	if (!IsValid(NewOwner))
	{
		UE_LOG(
			USf_BTDecorator_BlueprintBase_Log,
			Error,
			TEXT("Owner ist invalid"));
		return;
	}

	const AAIController* AIController = Cast<AAIController>(NewOwner);
	if (!IsValid(AIController))
	{
		UE_LOG(
			USf_BTDecorator_BlueprintBase_Log,
			Error,
			TEXT("Owner is not of type %s but of type %s instead"),*AAIController::StaticClass()->GetName(),*NewOwner->GetClass()->GetName());
		return;
	}
	
	if (!IsValid(AIController->GetPawn()))
	{
		UE_LOG(
			USf_BTDecorator_BlueprintBase_Log,
			Error,
			TEXT("Owner ist invalid"));
		return;
	}
	OwningNPCharacter = Cast<ASf_TP_Character>(AIController->GetPawn());

	if (!IsValid(OwningNPCharacter))
	{
		UE_LOG(
			USf_BTDecorator_BlueprintBase_Log,
			Error,
			TEXT("Require character of type %s but got character of type %s"),
			*ASf_TP_Character::StaticClass()->GetName(),*NewOwner->GetName());
	}
	
}


