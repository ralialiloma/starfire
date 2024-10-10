#pragma once
#include "CoreMinimal.h"
#include "Starfire/AI/EnemyFeature.h"
#include "EF_Cover.generated.h"


UCLASS(Blueprintable)
class STARFIRE_API UEF_Cover : public UEnemyFeature
{
	GENERATED_BODY()

#pragma region Functions
	UFUNCTION(BlueprintCallable)
	bool EnterCover();
	
	UFUNCTION(BlueprintCallable)
	bool IsCovered() const;
#pragma endregion
	
#pragma region Properties
protected:
	UPROPERTY(EditDefaultsOnly)
	float MinCoverHeight;

	UPROPERTY(EditDefaultsOnly)
	float CrouchCover ;
private:
	UPROPERTY()
	bool bIsInCover = false;
#pragma endregion


};