#pragma once
#include "CoreMinimal.h"
#include "CF_AreaBox_Config.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_AreaBox.generated.h"



UCLASS(Blueprintable)
class STARFIRE_API UCF_AreaBox : public USf_CharacterFeature
{
	GENERATED_BODY()

	
#pragma region Functions
public:
	virtual void Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig) override;

	

protected:
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY()
	const UCF_AreaBox_Config* AreaBox_Config;
#pragma endregion


};