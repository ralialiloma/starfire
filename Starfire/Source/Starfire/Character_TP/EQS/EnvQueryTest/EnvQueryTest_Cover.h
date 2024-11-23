


#pragma once
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_Cover.generated.h"


UCLASS()
class STARFIRE_API UEnvQueryTest_Cover : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	
public:
	UPROPERTY(EditDefaultsOnly, Category=Cover)
	float RadiusToCheck;

	UPROPERTY(EditDefaultsOnly, Category=Cover)
	float MinCoverPotential;

	UPROPERTY(EditDefaultsOnly, Category=Cover)
	float MaxCoverPotential;
	
public:
	UEnvQueryTest_Cover();
	virtual  void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;


private:
	bool HasCloseNavTarget(FVector CoverLocation) const;
	
};
