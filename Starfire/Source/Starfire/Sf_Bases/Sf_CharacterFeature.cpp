// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_CharacterFeature.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/Behaviour/BlackboardKeyHelperLibrary.h"

void USf_CharacterFeature::Initialize(ASf_TP_Character* Holder)
{
	OwningAIHolder = Holder;
	AController* Controller = Holder->GetController();

	if (!IsValid(Controller))
	{
		//UE_LOG(SfLog_CharacterFeature, Error, TEXT("Invalid Controller"))
		return;
	}
	
	OwningAIController = Cast<AAIController>(Controller);
	if (!IsValid(OwningAIController))
	{
		UE_LOG(
			SfLog_CharacterFeature,
			Error,
			TEXT("Invalid type. Needs to be AIController but its %s instead"),
			*Holder->GetController()->GetClass()->GetName())
	}
}


void USf_CharacterFeature::OnTick()
{
}

void USf_CharacterFeature::SetBlackboardFloatValue(EFloatBlackboardKey FloatBlackboardKey, float Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetFloatValue(BlackboardComponent,FloatBlackboardKey,Value);
}

void USf_CharacterFeature::SetBlackboardIntValue(EIntBlackboardKey IntBlackboardKey, int32 Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetIntValue(BlackboardComponent,IntBlackboardKey,Value);
}

void USf_CharacterFeature::SetBlackboardBoolValue(EBoolBlackboardKey BoolBlackboardKey, bool Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,BoolBlackboardKey,Value);
}

void USf_CharacterFeature::SetBlackboardVectorValue(ELocationBlackboardKey ActorBlackboardKey, FVector Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ActorBlackboardKey,Value);
}

USf_CharacterFeature* USf_CharacterFeature::GetFeatureByClass(TSubclassOf<USf_CharacterFeature> Class)
{
	return  GetOwningCharacter()->GetFeatureByClass(Class);
}



USf_TP_CharacterMovementComponent* USf_CharacterFeature::GetOwningSfMovement()
{
	return OwningAIHolder->GetSf_TP_CharacterMovement();
}

USf_Equipment* USf_CharacterFeature::GetOwningSfEquipment()
{
	return OwningAIHolder->GetSfEquipment();
}

ASf_TP_Character* USf_CharacterFeature::GetOwningCharacter() const
{
	return OwningAIHolder;
}

AAIController* USf_CharacterFeature::GetOwningAIController()
{
	return OwningAIController;
}

FVector USf_CharacterFeature::GetOwnerLocation() const
{
	return GetOwnerTransform().GetLocation();
}

FRotator USf_CharacterFeature::GetOwnerRotation() const
{
	return GetOwnerTransform().GetRotation().Rotator();
}

AWeaponBase* USf_CharacterFeature::GetOwningCharacterActiveWeapon()
{
	return GetOwningSfEquipment()->GetActiveWeapon();
}

FCollisionQueryParams USf_CharacterFeature::GetIgnoreCharacterParams()
{
	return GetOwningCharacter()->GetIgnoreCharacterParams();
}

TArray<AActor*> USf_CharacterFeature::GetIgnoreActors() const
{
	return OwningAIHolder->GetIgnoreActors();
}

FTransform USf_CharacterFeature::GetOwnerTransform() const
{
	return OwningAIHolder->GetTransform();
}

