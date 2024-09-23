// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Project/Weapon/FireBlocks.h"
#include "Project/Weapon/FireType.h"
#include "Project/Weapon/WeaponBase.h"
#include "Project/Utility/InputSignalType.h"
#include "Sf_Equipment.generated.h"


UCLASS(ClassGroup=(Character), meta=(BlueprintSpawnableComponent))
class PROJECT_API USF_Equipment : public USceneComponent
{
	GENERATED_BODY()

public:
	USF_Equipment();

protected:
	//Config
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly , Category = "Sockets",meta=(GetOptions="GetWeaponAttachmentSocketOptions"))
	FName WeaponAttachmentSocket;

private:
	//Transient

	UPROPERTY()
	TArray<AWeaponBase*> OwnedWeapons;

	UPROPERTY()
	AWeaponBase* EquippedWeapon;

	UPROPERTY()
	AActor* WeaponOwner;

protected:

	//Component
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitializeComponent() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	//Interface
public:
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsEquipped() const;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsAiming() const;

	UFUNCTION(BlueprintCallable)
	FWeaponAnimData GetAnimationData() const;

	UFUNCTION(BlueprintCallable)
	void AddWeapon(AWeaponBase* WeaponToAdd, bool Equip, int& Index);

	UFUNCTION(BlueprintCallable)
	void AddWeaponByClass(TSubclassOf<AWeaponBase> ActorClass, bool Equip, int& Index);

	UFUNCTION(BlueprintCallable)
	bool Fire(EInputSignalType InputSignal, EFireType FireType, FHitResult& OutHitResult,
	          TEnumAsByte<EFireBlock>& OutFireBlock);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanReload() const;
	
	UFUNCTION(BlueprintCallable)
	bool Reload();

	UFUNCTION(BlueprintCallable)
	void StopReloading();

	//Internal
private:
	bool GetSlot(AWeaponBase* WeaponBase,int& OutIndex) const;
	void AttachToParentMesh();

	UFUNCTION()
	TArray<FName> GetWeaponAttachmentSocketOptions();
};
