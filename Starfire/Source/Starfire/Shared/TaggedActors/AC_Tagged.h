#pragma once
#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagsManager.h"
#include "GameplayTagSourceMode.h"
#include "AC_Tagged.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARFIRE_API UAC_Tagged: public UActorComponent,public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UAC_Tagged(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
	UFUNCTION(BlueprintCallable)
	bool HasTag (
		UPARAM(meta=(Categories="Gameplay.Actor"))FGameplayTag Tag,
		bool bExactMatch = false,
		EGameplayTagSourceMode SourceType = EGameplayTagSourceMode::Both) const;
	UFUNCTION(BlueprintCallable)
	void GetGameplayTags(EGameplayTagSourceMode SourceType, FGameplayTagContainer& TagContainer) const;
	void GetStaticTags(FGameplayTagContainer& TagContainer) const;
	void GetDynamicTags(FGameplayTagContainer& TagContainer) const;

public:	
	UPROPERTY(BlueprintReadOnly, Category=Actor,meta=(Categories="Gameplay.Actor"))
	FGameplayTagContainer DynamicGameplayTags;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Actor,meta=(Categories="Gameplay.Actor"))
	FGameplayTagContainer StaticGameplayTags;
};



