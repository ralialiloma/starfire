#pragma once
#include "CoreMinimal.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature_Config.h"
#include "CF_Death_Config.generated.h"

class UPoseAnimInstance;
class AResource;

UCLASS(Blueprintable)
class STARFIRE_API UCF_Death_Config : public USf_CharacterFeature_Config
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<USf_CharacterFeature> GetFeatureType_Implementation() const override;
	
#pragma region Properties
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AResource> DroppedResourceClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRagdoll = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = bRagdoll))
	TSubclassOf<UPoseAnimInstance> AnimBlueprintClass = nullptr;
#pragma endregion


};


