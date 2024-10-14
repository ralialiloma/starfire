#pragma once
#include "CoreMinimal.h"
#include "Starfire/AI/EnemyFeature.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "EF_Death.generated.h"


DEFINE_LOG_CATEGORY_STATIC(EF_Death, Display, Display);

UCLASS(Blueprintable)
class STARFIRE_API UEF_Death : public UEnemyFeature
{
	GENERATED_BODY()

	
#pragma region Functions
public:
	UFUNCTION(BlueprintCallable, Category = "EnemyFeature|Death")
	void Kill();
#pragma endregion
	
#pragma region Properties
public:
private:
#pragma endregion


};