// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

class UBoxComponent;

UCLASS()
class STARFIRE_API ACheckPoint : public AActor
{
	GENERATED_BODY()

public:
	ACheckPoint();

	virtual void BeginPlay() override;

	//Subscriptions
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnCheckPointChange(FTransform NewCheckpoint, FTransform OldCheckpoint);

	UFUNCTION(BlueprintNativeEvent)
	void OnSetCheckPoint_Internal();
	UFUNCTION(BlueprintNativeEvent)
	void OnUnsetCheckPoint_Internal();
	
#pragma region Properties
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTextRenderComponent* TextRenderComponent;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UBoxComponent* BoxComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bRetriggerableCheckpoint = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<APawn> CheckTriggerPawnClass;
#pragma endregion

};
