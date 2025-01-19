
#include "CF_Death_Config.h"
#include "CF_Death.h"


UCF_Death_Config::UCF_Death_Config()
{
	DeathFXMessage = Sf_GameplayTags::Effects::Messages::TP::Death;
}

TSubclassOf<USf_CharacterFeature> UCF_Death_Config::GetFeatureType_Implementation() const
{
	return UCF_Death::StaticClass();
}

