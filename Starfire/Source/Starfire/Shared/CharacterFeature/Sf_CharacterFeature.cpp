// Fill out your copyright notice in the Description page of Project Settings.


#include "Sf_CharacterFeature.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Character_TP/Behaviour/BlackboardKeyHelperLibrary.h"
#include "Starfire/Utility/Debug/DebugFunctionLibrary.h"

void USf_CharacterFeature::Initialize(ASf_TP_Character* OwningCharacterIn,const USf_CharacterFeature_Config* InConfig)
{
	OwningCharacter = OwningCharacterIn;
	AController* Controller = OwningCharacter->GetController();

	Config = InConfig;

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
			*OwningCharacterIn->GetController()->GetClass()->GetName())
	}
}


void USf_CharacterFeature::OnBeginPlay()
{
}


void USf_CharacterFeature::OnTick(float OnTick)
{
}

void USf_CharacterFeature::OnEndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void USf_CharacterFeature::SetBlackboardFloatValue(EFloatBlackboardKey FloatBlackboardKey, float Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningCharacter);
	UBlackboardKeyHelperLibrary::SetFloatValue(BlackboardComponent,FloatBlackboardKey,Value);
}

void USf_CharacterFeature::SetBlackboardIntValue(EIntBlackboardKey IntBlackboardKey, int32 Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningCharacter);
	UBlackboardKeyHelperLibrary::SetIntValue(BlackboardComponent,IntBlackboardKey,Value);
}

void USf_CharacterFeature::SetBlackboardBoolValue(EBoolBlackboardKey BoolBlackboardKey, bool Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningCharacter);
	UBlackboardKeyHelperLibrary::SetBoolValue(BlackboardComponent,BoolBlackboardKey,Value);
}

void USf_CharacterFeature::SetBlackboardVectorValue(ELocationBlackboardKey ActorBlackboardKey, FVector Value)
{
	UBlackboardComponent* BlackboardComponent = UAIBlueprintHelperLibrary::GetBlackboard(OwningCharacter);
	UBlackboardKeyHelperLibrary::SetVectorValue(BlackboardComponent,ActorBlackboardKey,Value);
}

USf_CharacterFeature* USf_CharacterFeature::GetFeatureByClass(TSubclassOf<USf_CharacterFeature> Class)
{
	return  GetOwningCharacter()->GetFeatureByClass(Class);
}



USf_TP_CharacterMovementComponent* USf_CharacterFeature::GetOwningSfMovement()
{
	return OwningCharacter->GetSf_TP_CharacterMovement();
}

USf_Equipment* USf_CharacterFeature::GetOwningSfEquipment()
{
	if (!this)
		return nullptr;

	if (!IsValid(OwningCharacter))
		return nullptr;
	
	return OwningCharacter->GetSfEquipment();
}

ASf_TP_Character* USf_CharacterFeature::GetOwningCharacter() const
{
	return OwningCharacter;
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

FCollisionQueryParams USf_CharacterFeature::GetIgnoreCharacterParams() const
{
	return GetOwningCharacter()->GetIgnoreCharacterParams();
}

TArray<AActor*> USf_CharacterFeature::GetIgnoreActors() const
{
	return OwningCharacter->GetIgnoreActors();
}

void USf_CharacterFeature::ThrowInvalidConfigError(TSubclassOf<USf_CharacterFeature_Config> ConfigToCastTo)
{
	UDebugFunctionLibrary::Sf_ThrowError(this,FString::Printf(TEXT("Invalid %s Config!"),*ConfigToCastTo->GetName()));
}

const USf_CharacterFeature_Config* USf_CharacterFeature::GetConfig() const
{
	return Config;
}

FTransform USf_CharacterFeature::GetOwnerTransform() const
{
	return OwningCharacter->GetTransform();
}

