
#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnvQueryTest_LookForCoverWall.generated.h"




UCLASS()
class STARFIRE_API UEnvQueryTest_LookForCoverWall : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()
	

public:
	UEnvQueryTest_LookForCoverWall();
	virtual  void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;

	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;

	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	TSubclassOf<UEnvQueryContext> Ignore;
	
	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	TEnumAsByte<EDrawDebugTrace::Type> DebugDrawTraceType;
	
	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	TSubclassOf<UEnvQueryContext> HideFrom;

	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	float MaxDistanceToCover = 100.f;

	UPROPERTY(EditDefaultsOnly, Category=WallCover)
	float MinDistanceToCover = 20.f;
	
};
