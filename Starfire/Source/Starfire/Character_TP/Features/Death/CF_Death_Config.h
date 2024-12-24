#pragma once
#include "CoreMinimal.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature_Config.h"
#include "CF_Death_Config.generated.h"

class AResource;

UCLASS(Blueprintable)
class STARFIRE_API UCF_Death_Config : public USf_CharacterFeature_Config
{
	GENERATED_BODY()

public:
	virtual TSubclassOf<USf_CharacterFeature> GetFeatureType() const override;
	
#pragma region Properties
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AResource> DroppedResourceClass;
#pragma endregion


};


