// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFeature.h"
#include "Sf_NPCharacter.h"
#include "Behaviour/BlackboardKeyHelperLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void UEnemyFeature::Initialize(ASf_NPCharacter* Holder)
{
	this->OwningAIHolder = Holder;
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

UCharacterMovementComponent* UEnemyFeature::GetCharacterMovement()
{
	return OwningAIHolder->GetCharacterMovement();
}

