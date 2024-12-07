// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sf_PatrolAreaMarker.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Sf_PatrolArea.generated.h"

UCLASS()
class STARFIRE_API ASf_PatrolArea : public AActor
{
	GENERATED_BODY()

public:
	ASf_PatrolArea();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInBox(const FVector& LocationToTest) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOccupied() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnRegisterMarker(ASf_PatrolAreaMarker* NewMarker);
	UFUNCTION(BlueprintNativeEvent)
	void OnUnregistermarker(ASf_PatrolAreaMarker* OldMarker);
	UFUNCTION(BlueprintCallable)
	bool TryRegisterMarker(ASf_PatrolAreaMarker* Marker);
	UFUNCTION(BlueprintCallable)
	void UnregisterMarker(ASf_PatrolAreaMarker* Marker);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ASf_PatrolAreaMarker*> GetCurrentMarkers() const;

	UFUNCTION(BlueprintCallable)
	void ValidateMarkers();

	void ImportActors();

	void OnCharacterDeath();

protected:
	void GetActorsInsideBox(TArray<AActor*>& OutActors) const;

protected:
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UBoxComponent* Box;

	UPROPERTY(BlueprintReadOnly)
	TArray<ASf_PatrolAreaMarker*> Markers;

	UPROPERTY(BlueprintReadOnly)
	ASf_TP_Character* Character;

};