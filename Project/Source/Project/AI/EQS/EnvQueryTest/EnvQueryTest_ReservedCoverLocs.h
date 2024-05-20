


#pragma once
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_ReservedCoverLocs.generated.h"


UCLASS()
class PROJECT_API UEnvQueryTest_ReservedCoverLocs : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	
public:
	UPROPERTY(EditDefaultsOnly, Category=ReservedCover)
	float RadiusToCheck;
	UPROPERTY(EditDefaultsOnly, Category=ReservedCover)
	bool CoverShouldBeValid = true;

public:
	UEnvQueryTest_ReservedCoverLocs();
	virtual  void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;


private:
	bool InReservedCover(FVector CoverLocation) const;
	
};
