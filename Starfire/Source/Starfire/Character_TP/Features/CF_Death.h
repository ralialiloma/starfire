#pragma once
#include "CoreMinimal.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "Starfire/Sf_Bases/Sf_CharacterFeature.h"
#include "CF_Death.generated.h"


class AResource;
DEFINE_LOG_CATEGORY_STATIC(EF_Death, Display, Display);

UCLASS(Blueprintable)
class STARFIRE_API UCF_Death : public USf_CharacterFeature
{
	GENERATED_BODY()

	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Death")
	void Kill();
#pragma endregion
	
#pragma region Properties
public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AResource> DroppedResourceClass;
private:
#pragma endregion


};