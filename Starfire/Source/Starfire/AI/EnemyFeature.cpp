// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFeature.h"
#include "Sf_NPCharacter.h"
#include "Behaviour/BlackboardKeyHelperLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void UEnemyFeature::Initialize(ASf_NPCharacter* Holder)
{
	OwningAIHolder = Holder;
	AController* Controller = Holder->GetController();

	if (!IsValid(Controller))
	{
		UE_LOG(EnemyFeature, Error, TEXT("Invalid Controller"))
		return;
	}
	
	OwningAIController = Cast<AAIController>(Holder->GetController());
	if (!IsValid(OwningAIController))
	{
		UE_LOG(
			EnemyFeature,
			Error,
			TEXT("Invalid type. Needs to be AIController but its %s instead"),
			*OwningAIController->GetClass()->GetName())
		return;
	}
}

void UEnemyFeature::OnTick()
{
}

void UEnemyFeature::SetBlackboardFloatValue(EFloatBlackboardKey FloatBlackboardKey, float Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetFloatValue(BlackboardComponent,FloatBlackboardKey,Value);
}

void UEnemyFeature::SetBlackboardIntValue(EIntBlackboardKey IntBlackboardKey, int32 Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetIntValue(BlackboardComponent,IntBlackboardKey,Value);
}

void UEnemyFeature::SetBlackboardBoolValue(EBoolBlackboardKey BoolBlackboardKey, bool Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,BoolBlackboardKey,Value);
}

void UEnemyFeature::SetBlackboardVectorValue(ELocationBlackboardKey ActorBlackboardKey, FVector Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ActorBlackboardKey,Value);
}

UEnemyFeature* UEnemyFeature::GetFeatureByClass(TSubclassOf<UEnemyFeature> Class)
{
	return  GetOwningCharacter()->GetFeatureByClass(Class);
}



USf_TP_CharacterMovementComponent* UEnemyFeature::GetOwningSfMovement()
{
	return OwningAIHolder->GetSfMovement();
}

USF_Equipment* UEnemyFeature::GetOwningSfEquipment()
{
	return OwningAIHolder->GetSfEquipment();
}

ASf_NPCharacter* UEnemyFeature::GetOwningCharacter()
{
	return OwningAIHolder;
}

AAIController* UEnemyFeature::GetOwningAIController()
{
	return OwningAIController;
}

FVector UEnemyFeature::GetOwnerLocation() const
{
	return GetOwnerTransform().GetLocation();
}

FRotator UEnemyFeature::GetOwnerRotation() const
{
	return GetOwnerTransform().GetRotation().Rotator();
}

AWeaponBase* UEnemyFeature::GetOwningCharacterActiveWeapon()
{
	return GetOwningSfEquipment()->GetActiveWeapon();
}

FCollisionQueryParams UEnemyFeature::GetIgnoreCharacterParams()
{
	return GetOwningCharacter()->GetIgnoreCharacterParams();
}

TArray<AActor*> UEnemyFeature::GetIgnoreActors() const
{
	return OwningAIHolder->GetIgnoreActors();
}

FTransform UEnemyFeature::GetOwnerTransform() const
{
	return OwningAIHolder->GetTransform();
}

