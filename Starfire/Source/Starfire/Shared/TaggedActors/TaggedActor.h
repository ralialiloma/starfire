#pragma once
#include "Starfire/Shared/TaggedActors/AC_Tagged.h"
#include "TaggedActor.generated.h"


UCLASS(BlueprintType)
class STARFIRE_API ATaggedActor : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ATaggedActor(const FObjectInitializer& ObjectInitializer);
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable)
	void GetGameplayTags(const EGameplayTagSourceMode SourceType, FGameplayTagContainer& TagContainer) const;
	UFUNCTION(BlueprintCallable)
	bool HasTag (
		UPARAM(meta=(Categories="Gameplay.Actor"))FGameplayTag Tag,
		bool bExactMatch = false,
		EGameplayTagSourceMode SourceType = EGameplayTagSourceMode::Both) const;
	UFUNCTION(BlueprintCallable)
	void AddTag(UPARAM(meta=(Categories="Gameplay.Actor"))FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable)
	void RemoveTag(UPARAM(meta=(Categories="Gameplay.Actor"))FGameplayTag TagToRemove);
	
protected:
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	UAC_Tagged* TaggedComp;
};
