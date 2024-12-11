#pragma once
#include "CoreMinimal.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_EQSData.generated.h"

UCLASS(Blueprintable)
class STARFIRE_API UCF_EQS_Data : public USf_CharacterFeature
{
	GENERATED_BODY()

	
#pragma region Functions
public:
	virtual void Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig) override;
	virtual void OnBeginPlay() override;
protected:
#pragma endregion
	
#pragma region Properties
protected:
	TMap<ELocationBlackboardKey,FGuid> ScheduledEQS;
#pragma endregion


};