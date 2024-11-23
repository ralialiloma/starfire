


#pragma once
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_RemoveNavTargets.generated.h"


UCLASS()
class STARFIRE_API UEnvQueryTest_RemoveNavTargets : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	
public:
	UPROPERTY(EditDefaultsOnly, Category=ReservedCover)
	float RadiusToCheck;
	UPROPERTY(EditDefaultsOnly, Category=ReservedCover)
	bool CoverShouldBeValid = true;

public:
	UEnvQueryTest_RemoveNavTargets();
	virtual  void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;


private:
	bool HasCloseNavTarget(FVector CoverLocation) const;
	
};
