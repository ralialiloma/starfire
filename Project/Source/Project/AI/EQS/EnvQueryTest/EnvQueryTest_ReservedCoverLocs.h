


#pragma once
#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_ReservedCoverLocs.generated.h"


UCLASS()
class PROJECT_API UEnvQueryTest__ReservedCoverLocs : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()
	

public:
	UEnvQueryTest__ReservedCoverLocs();
	virtual  void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
	
};
