#pragma once
#include "Enum/LocomotionDirection.h"
#include "DirectionalAnimationSet.generated.h"

USTRUCT(BlueprintType)
struct FDirectionalAnimationSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ArraySizeEnum = "STARFIRE.ELocomotionDirection", EditFixedOrder, EditFixedSize))
	TMap<ELocomotionDirection, UAnimSequence*> DirectionalSequences
		= TMap<ELocomotionDirection, UAnimSequence*>
		{
			TPair<ELocomotionDirection, UAnimSequence*>(ELocomotionDirection::F, nullptr),
			TPair<ELocomotionDirection, UAnimSequence*>(ELocomotionDirection::B, nullptr),
			TPair<ELocomotionDirection, UAnimSequence*>(ELocomotionDirection::L, nullptr),
			TPair<ELocomotionDirection, UAnimSequence*>(ELocomotionDirection::R, nullptr),
		};
};
