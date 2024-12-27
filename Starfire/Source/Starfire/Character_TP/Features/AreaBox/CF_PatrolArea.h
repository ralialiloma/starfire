#pragma once
#include "CoreMinimal.h"
#include "CF_PatrolArea_Config.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "Starfire/Character_TP/PatrolArea/Sf_PatrolArea.h"
#include "Starfire/Shared/CharacterFeature/Sf_CharacterFeature.h"
#include "CF_PatrolArea.generated.h"



UCLASS(Blueprintable)
class STARFIRE_API UCF_PatrolArea : public USf_CharacterFeature
{
	GENERATED_BODY()

	
#pragma region Functions
public:
	virtual void Initialize(ASf_TP_Character* Holder, const USf_CharacterFeature_Config* InConfig) override;
	virtual void OnBeginPlay_Implementation() override;

	void OnMoveFinished();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	ASf_PatrolArea* GetCurrentPatrolArea();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FGameplayTagContainer GetActivePatrolPointTags() const;
protected:
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY()
	const UCF_PatrolArea_Config* AreaBox_Config;
	
	UPROPERTY()
	FGameplayTagContainer ActivePatrolPointTags;

	UPROPERTY()
	ASf_PatrolArea* CurrentPatrolArea;
#pragma endregion


};