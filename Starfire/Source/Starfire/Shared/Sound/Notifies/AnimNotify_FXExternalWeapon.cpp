// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FXExternalWeapon.h"

#include "Starfire/Sf_Bases/Components/Sf_Equipment.h"


void UAnimNotify_FXExternalWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!GetWorld())
		return;
	

	AActor* ActorCharacter = MeshComp->GetOwner();
	if (!ActorCharacter)
		return;
	
	USf_Equipment* EquipmentComp = ActorCharacter->GetComponentByClass<USf_Equipment>();
	if (!EquipmentComp)
		return;

	AWeaponBase* Weapon = EquipmentComp->GetActiveWeapon();
	if (!Weapon)
		return;

	USkeletalMeshComponent* TargetMesh = Weapon->GetSkeletalMesh();
	if (!TargetMesh)
		return;
	
	Super::Notify(TargetMesh, Animation, EventReference);
}
