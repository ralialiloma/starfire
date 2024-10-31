#pragma once
#include "GameFramework/ProjectileMovementComponent.h"
#include "Starfire/Shared/Weapon/WeaponBase.h"
#include "ThrowingKnife.generated.h"


UCLASS(BlueprintType)
class AThrowingKnife: public AWeaponBase
{
	GENERATED_BODY()
public:
	AThrowingKnife(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnEquip() override;
	virtual void SetWeaponActive(const bool Active, AWeaponBase* PreviousWeapon) override;

	virtual bool CanFire(EInputSignalType InputSignal, EFireType FireType, EFireBlock& OutBlock) override;
	virtual bool CanMelee() override;
	virtual bool CanReload() override;
	virtual bool CanAim() override;
#pragma region Functions
public:
	void Throw();
protected:
	virtual FVector GetAimLocation();
#pragma endregion

#pragma region Properties
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ThrowingSpeed = 4100.f;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float WeaponLength = 70.f;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;
private:
	UPROPERTY()
	bool bInAir = false;
	UPROPERTY()
	APawn* OldOwner = nullptr;
#pragma endregion
};
