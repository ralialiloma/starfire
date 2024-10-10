// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFeature.h"
#include "Sf_NPCharacter.h"
#include "Behaviour/BlackboardKeyHelperLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void UEnemyFeature::Initialize(ASf_NPCharacter* Holder)
{
	this->OwningAIHolder = Holder;
	this->OwningAIController = Cast<AAIController>(Holder->GetController());
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

void UEnemyFeature::SetBlackboardActorValue(EActorBlackboardKey ActorBlackboardKey, AActor* Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningAIHolder);
	UBlackboardKeyHelperLibrary::SetActorValue(BlackboardComponent,ActorBlackboardKey,Value);
}

UCharacterMovementComponent* UEnemyFeature::GetOwningSfMovement()
{
	return OwningAIHolder->GetCharacterMovement();
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
	return GetOwningCharacter()->GetIgnoreActors();
}

FTransform UEnemyFeature::GetOwnerTransform() const
{
	return OwningAIHolder->GetTransform();
}

