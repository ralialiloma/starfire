#pragma once
#include "Sf_CharacterFeature.h"
#include "Starfire/Sf_Bases/Sf_Object.h"
#include "Sf_CharacterFeature_Config.generated.h"

class ASf_TP_Character;

UCLASS(BlueprintType,Blueprintable,DefaultToInstanced,EditInlineNew,Abstract)
class STARFIRE_API USf_CharacterFeature_Config : public USf_Object
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<USf_CharacterFeature> GetFeatureType() const;
	USf_CharacterFeature* MakeFeature(ASf_TP_Character* OwningCharacter) const;
	
};
