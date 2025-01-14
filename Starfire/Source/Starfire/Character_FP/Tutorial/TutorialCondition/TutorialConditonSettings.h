#pragma once
#include "Enums/ConditionMatchType.h"

#include "TutorialConditonSettings.generated.h"

class UTutorialConditions;

USTRUCT(BlueprintType)
struct FTutorialConditionSettings
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EConditionMatchType MatchType = EConditionMatchType::All;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced)
	TArray<UTutorialConditions*> TutorialConditions;

	bool Evaluate(ATutorialManager* TutorialManager);
};

inline bool FTutorialConditionSettings::Evaluate(ATutorialManager* TutorialManager)
{
	for (auto Condition : TutorialConditions)
	{
		if (!Condition)
			continue;
		
		const bool ConditionResult = Condition->EvaluateCondition_Implementation(TutorialManager);

		switch (MatchType) {
		case EConditionMatchType::Any:
			if (ConditionResult)
				return true;
			break;
		case EConditionMatchType::All:
			if (!ConditionResult)
				return false;
			break;
		case EConditionMatchType::NAny:
			if (!ConditionResult)
				return true;
			break;
		case EConditionMatchType::None:
			if (ConditionResult)
				return false;
			break;
		}
	}

	switch (MatchType) {
	case EConditionMatchType::Any:
			return false;
	case EConditionMatchType::All:
			return true;
	case EConditionMatchType::NAny:
			return false;
	case EConditionMatchType::None:
			return true;
	}

	//Should never get here
	return true;
}
