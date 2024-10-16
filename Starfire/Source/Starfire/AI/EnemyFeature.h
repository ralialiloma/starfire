// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Behaviour/BlackboardKeyHelperLibrary.h"
#include "Starfire/Character/Sf_Equipment.h"
#include "CollisionQueryParams.h"
#include "CoreMinimal.h"
#include "Sf_TP_CharacterMovementComponent.h"
#include "Starfire/Sf_Bases/Sf_Object.h"
#include "EnemyFeature.generated.h"

class ASf_NPCharacter;

DEFINE_LOG_CATEGORY_STATIC(EnemyFeature, Display, Display);


UCLASS(BlueprintType,Blueprintable)
class STARFIRE_API UEnemyFeature : public USf_Object
{
	GENERATED_BODY()


#pragma region Functions
public:
	virtual void Initialize(ASf_NPCharacter* Holder);
	virtual void OnTick();
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
	UEnemyFeature* GetFeatureByClass(TSubclassOf<UEnemyFeature> Class);
	
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Character")
	USf_TP_CharacterMovementComponent* GetOwningSfMovement();
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Character")
	ASf_NPCharacter* GetOwningCharacter();
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Character")
	AAIController* GetOwningAIController();

	UFUNCTION(BlueprintCallable,BlueprintPure , Category = "Character")      
	FTransform GetOwnerTransform() const;
	UFUNCTION(BlueprintCallable,BlueprintPure,  Category = "Character")
	FVector GetOwnerLocation() const;
	UFUNCTION(BlueprintCallable,BlueprintPure,  Category = "Character")
	FRotator GetOwnerRotation() const;

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Character")
	USF_Equipment* GetOwningSfEquipment();
	UFUNCTION(BlueprintCallable,BlueprintPure,  Category = "Character")
	AWeaponBase* GetOwningCharacterActiveWeapon();
	
	FCollisionQueryParams GetIgnoreCharacterParams();

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Character")
	TArray<AActor*> GetIgnoreActors() const;
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	bool bRunInTick = false;

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	float UpdateRateInSeconds = 1;
private:
	UPROPERTY()
	ASf_NPCharacter* OwningAIHolder = nullptr;

	UPROPERTY()
	AAIController* OwningAIController = nullptr;
#pragma endregion
	
};
