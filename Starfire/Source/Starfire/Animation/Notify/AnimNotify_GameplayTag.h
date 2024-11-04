#pragma once
#include "GameplayTagContainer.h"
#include "AnimNotify_GameplayTag.generated.h"

DEFINE_LOG_CATEGORY_STATIC(SF_WeaponAnimationReceiver, Display, Display);

UCLASS(const, hidecategories=Object, collapsecategories, Config = Game, meta=(DisplayName="Notify Gameplaytag"))
class UAnimNotify_Gameplaytag: public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_Gameplaytag();
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(const USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#pragma region Properties
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn = true,Categories = "Animation.Notify"))
	FGameplayTag GameplayTag;
#pragma endregion

};
