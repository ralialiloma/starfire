#include "AnimNotify_GameplayTag.h"
#include "Starfire/Character_FP/Components/AnimDataController_FP.h"


UAnimNotify_Gameplaytag::UAnimNotify_Gameplaytag()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Green;
#endif
}

FString UAnimNotify_Gameplaytag::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("Notfiy Tag:[%s]"),*GameplayTag.GetTagName().ToString()); 
}

void UAnimNotify_Gameplaytag::Notify(const USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!GetWorld())
		return;
	
	const AActor* Actor =  MeshComp->GetOwner();
	if (!IsValid(Actor))
		return;
	
	USf_AnimHelper* AnimController =  Actor->GetComponentByClass<USf_AnimHelper>();
	if (!IsValid(AnimController))
		return;

	AnimController->OnAnimNotifyEvent(GameplayTag);
}
