// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "CollisionQueryParams.h"
#include "CoreMinimal.h"
#include "Starfire/Character_TP/Behaviour/BlackboardKeyHelperLibrary.h"
#include "Starfire/Character_TP/Components/Sf_TP_CharacterMovementComponent.h"
#include "Starfire/Sf_Bases/Sf_Object.h"
#include "Sf_CharacterFeature.generated.h"

class ASf_TP_Character;
class USf_CharacterFeature_Config;

DEFINE_LOG_CATEGORY_STATIC(SfLog_CharacterFeature, Display, Display);

#define VALIDATE_CONFIG(Type, ConfigVar) \
	ConfigVar = Cast<Type>(GetConfig()); \
	if (!IsValid(ConfigVar)) \
	{ \
		ThrowInvalidConfigError(Type::StaticClass());\
		return; \
	}

UCLASS(BlueprintType,Blueprintable)
class STARFIRE_API USf_CharacterFeature : public USf_Object
{
	GENERATED_BODY()


#pragma region Functions
public:
	virtual void Initialize(ASf_TP_Character* OwningCharacterIn, const USf_CharacterFeature_Config* InConfig);
	virtual void OnBeginPlay();
	virtual void OnTick(float OnTick);
	virtual void OnEndPlay(const EEndPlayReason::Type EndPlayReason);
protected:
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void SetBlackboardFloatValue(EFloatBlackboardKey FloatBlackboardKey, float Value);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void SetBlackboardIntValue(EIntBlackboardKey IntBlackboardKey, int32 Value);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void SetBlackboardBoolValue(EBoolBlackboardKey BoolBlackboardKey, bool Value);
	UFUNCTION(BlueprintCallable, Category = "Blackboard|Setters")
	void SetBlackboardVectorValue(ELocationBlackboardKey ActorBlackboardKey, FVector Value);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyFeatures", meta = (DeterminesOutputType = "Class"))
	USf_CharacterFeature* GetFeatureByClass(TSubclassOf<USf_CharacterFeature> Class);
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Character")
	USf_TP_CharacterMovementComponent* GetOwningSfMovement();
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Character")
	ASf_TP_Character* GetOwningCharacter() const;
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Character")
	AAIController* GetOwningAIController();
	const USf_CharacterFeature_Config* GetConfig() const;

	UFUNCTION(BlueprintCallable,BlueprintPure , Category = "Character")      
	FTransform GetOwnerTransform() const;
	UFUNCTION(BlueprintCallable,BlueprintPure,  Category = "Character")
	FVector GetOwnerLocation() const;
	UFUNCTION(BlueprintCallable,BlueprintPure,  Category = "Character")
	FRotator GetOwnerRotation() const;

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Character")
	USf_Equipment* GetOwningSfEquipment();
	UFUNCTION(BlueprintCallable,BlueprintPure,  Category = "Character")
	AWeaponBase* GetOwningCharacterActiveWeapon();
	
	FCollisionQueryParams GetIgnoreCharacterParams() const;

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Character")
	TArray<AActor*> GetIgnoreActors() const;

	UFUNCTION(BlueprintCallable,Category = "Character Feature")
	void ThrowInvalidConfigError(TSubclassOf<USf_CharacterFeature_Config> ConfigToCastTo);

#pragma endregion

#pragma region Properties
public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	bool bRunInTick = false;

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	float UpdateRateInSeconds = 1;
private:
	UPROPERTY()
	ASf_TP_Character* OwningCharacter = nullptr;

	UPROPERTY()
	AAIController* OwningAIController = nullptr;

	UPROPERTY()
	const USf_CharacterFeature_Config* Config = nullptr;
#pragma endregion
	
};
