#include "Sf_CharacterFeature_Config.h"
#include "Starfire/Character_TP/Sf_TP_Character.h"
#include "Starfire/Utility/Debug/SF_DebugFunctionLibrary.h"


TSubclassOf<USf_CharacterFeature> USf_CharacterFeature_Config::GetFeatureType() const
{
	return nullptr;
}


USf_CharacterFeature* USf_CharacterFeature_Config::MakeFeature(ASf_TP_Character* OwningCharacter) const
{
	if (!IsValid(OwningCharacter))
	{
		USF_DebugFunctionLibrary::Sf_ThrowError(this, "Invalid OwningCharacter provided!");
		return nullptr;
	}
	
	TSubclassOf<USf_CharacterFeature> FeatureType = GetFeatureType();
	if (!IsValid(FeatureType))
	{
		USF_DebugFunctionLibrary::Sf_ThrowError(this,"Invalid Feature Type!");
		return nullptr;
	}
	
	USf_CharacterFeature* NewFeature =   NewObject<USf_CharacterFeature>(OwningCharacter,FeatureType);

	if (!IsValid(NewFeature))
	{
		USF_DebugFunctionLibrary::Sf_ThrowError(this,"Failed to create feature!");
		return nullptr;
	}
	
	NewFeature->Initialize(OwningCharacter,this);
	return NewFeature;
}

