#pragma once
#include "AIController.h"
#include "Sf_TP_Controller.generated.h"

UCLASS()
class ASf_TP_Controller: public AAIController
{
	GENERATED_BODY()
public:
	ASf_TP_Controller(const FObjectInitializer& ObjectInitializer);
	
#pragma region Functions

#pragma endregion

#pragma region Properties
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;
#pragma endregion
};
