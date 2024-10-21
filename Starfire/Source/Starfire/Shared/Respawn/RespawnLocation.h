// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "RespawnLocation.generated.h"

UCLASS()
class STARFIRE_API ARespawnLocation : public AActor
{
	GENERATED_BODY()

public:
	ARespawnLocation();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

#pragma region Functions
public:
	UFUNCTION(BlueprintCallable)
	static ARespawnLocation* GetClosestSpawnLocation(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static FTransform GetClosestRespawnTransform(UObject* WorldContextObject);
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UTextRenderComponent* TextRenderComponent;
#pragma endregion

};
