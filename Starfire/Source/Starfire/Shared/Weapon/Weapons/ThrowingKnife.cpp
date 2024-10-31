#include "ThrowingKnife.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Starfire/Sf_Bases/Components/Sf_Equipment.h"
#include "Starfire/Shared/Weapon/Interfaces/WeaponOwner.h"
#include "Starfire/Utility/AsyncUtility.h"
#include "Starfire/Utility/Sf_FunctionLibrary.h"

AThrowingKnife::AThrowingKnife(const FObjectInitializer& ObjectInitializer) : AWeaponBase(ObjectInitializer) 
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->bAutoActivate = false;
}

void AThrowingKnife::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bInAir)
		return;

	const FVector Start = GetActorLocation();
	FVector VelocityDir = ProjectileMovementComponent->Velocity;
	VelocityDir.Normalize();
	const FVector End = Start + VelocityDir*WeaponLength;

	FHitResult HitResult;
	const TArray<AActor*> IgnoredActors = TArray<AActor*>{this,OldOwner};

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		WeaponConfig.TraceTypeQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::Persistent,
		HitResult,
		true,
		FColor::Green,
		FColor::Yellow,
		10.f);

	if (HitResult.bBlockingHit)
	{
		if (HitResult.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, HitResult.GetActor()->GetName());
		}
		bInAir = false;
		ProjectileMovementComponent->StopMovementImmediately();
		ProjectileMovementComponent->ProjectileGravityScale = 0;

		ApplyDamage(HitResult);
	}
	
}

void AThrowingKnife::OnEquip()
{
	Super::OnEquip();
}

void AThrowingKnife::SetWeaponActive(const bool Active, AWeaponBase* PreviousWeapon)
{
	Super::SetWeaponActive(Active, PreviousWeapon);

	if (!Active)
		return;

	ForbiddenActions.Add(Sf_GameplayTags::Gameplay::Weapon::Action::ActivateWeapon);
	ForbiddenActions.Add(Sf_GameplayTags::Gameplay::Weapon::Action::EquipWeapon);
	ForbiddenActions.Add(Sf_GameplayTags::Gameplay::Weapon::Action::UnequipWeapon);
	ForbiddenActions.Add(Sf_GameplayTags::Gameplay::Weapon::Action::AddWeapon);
	ForbiddenActions.Add(Sf_GameplayTags::Gameplay::Weapon::Action::RemoveWeapon);
	
	TWeakObjectPtr<AThrowingKnife> WeakSelf = this;
	TWeakObjectPtr<AWeaponBase> WeakPrevWeapon = PreviousWeapon;
	
	float TimeForAnimation =  WeakSelf->ExecuteAnimationAndReturnAnimLength(EWeaponAnimationEventType::Fire);
	FAsyncUtility::RunOnAnyThread<void>(WeakSelf,[WeakSelf,WeakPrevWeapon,TimeForAnimation]()->void
	{
		FAsyncUtility::WaitForSeconds(TimeForAnimation,WeakSelf,0.01f);

		FAsyncUtility::RunOnGameThread<void>(WeakSelf,[WeakSelf,WeakPrevWeapon]
		{
			WeakSelf->Throw();
			WeakSelf->ForbiddenActions.Empty();
			WeakSelf->GetOwningSfEquipmentComp()->ActivateWeapon(WeakPrevWeapon.Get());
		});
		
	});
	
}

bool AThrowingKnife::CanFire(EInputSignalType InputSignal, EFireType FireType, EFireBlock& OutBlock)
{
	OutBlock = EFireBlock::Incompatible;
	return false;
}

bool AThrowingKnife::CanMelee()
{
	return false;
}

bool AThrowingKnife::CanReload()
{
	return false;
}

bool AThrowingKnife::CanAim()
{
	return false;
}


void AThrowingKnife::Throw()
{
	OldOwner = GetWeaponOwner();
	const FVector AimLocation = 	GetAimLocation();
	const FVector KnifeLocation = GetActorLocation();
	const FVector Direction = (AimLocation - KnifeLocation).GetSafeNormal();

	ForbiddenActions.Remove(Sf_GameplayTags::Gameplay::Weapon::Action::RemoveWeapon);
	GetOwningSfEquipmentComp()->RemoveWeapon(this);
	ForbiddenActions.Add(Sf_GameplayTags::Gameplay::Weapon::Action::RemoveWeapon);
	
	ProjectileMovementComponent->Activate();
	ProjectileMovementComponent->Velocity = Direction * ThrowingSpeed;
	bInAir = true;
}

FVector AThrowingKnife::GetAimLocation()
{
	const FTransform FireTransform = IWeaponOwner::Execute_GetFireTransform(GetWeaponOwner());
	const UE::Math::TQuat<double> Rotation = FireTransform.GetRotation();
	const FVector ForwardVector = Rotation.GetForwardVector();

	const FVector Start = FireTransform.GetLocation();
	const FVector End = Start + (ForwardVector * 10000.0f);

	FHitResult HitResult;
	const TArray<AActor*> IgnoredActors = TArray<AActor*>{this,GetWeaponOwner()};
	
	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		WeaponConfig.TraceTypeQuery,
		false,
		IgnoredActors,
		EDrawDebugTrace::Persistent,
		HitResult,
		true,
		FColor::Green,
		FColor::Yellow,
		10.f);

	if (HitResult.bBlockingHit)
		return HitResult.ImpactPoint;
	
	return End;
}
