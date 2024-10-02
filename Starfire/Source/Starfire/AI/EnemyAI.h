// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFeature.h"
#include "GameFramework/Character.h"
#include "Starfire/DamageSystem/Sf_DamageReceiver.h"
#include "EnemyAI.generated.h"

UCLASS()
class STARFIRE_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere)
	TArray<UEnemyFeature*> Features;

protected:
	UPROPERTY(BlueprintReadOnly)
	USf_DamageReceiver* SfDamageReceiver;

	//Interface
public:
		//Features
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "EnemyFeatures", meta = (DeterminesOutputType = "Class"))
	UEnemyFeature* GetFeatureByClass(TSubclassOf<UEnemyFeature> Class);

	UFUNCTION(BlueprintCallable, Category = "EnemyFeatures")
	bool TryAddFeature(UEnemyFeature* Feature);

		//Components
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USf_DamageReceiver* GetSfDamageReceiver();

	//Actor
public:
	AEnemyAI();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	




};
