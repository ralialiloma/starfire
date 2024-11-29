#include "MeleeInfo.h"

FMeleeInfo::FMeleeInfo()
{
	/*Location = FVector::ZeroVector;
	Extent = FVector::ZeroVector;
	Rotation = FRotator::ZeroRotator;
	Direction = FVector::ZeroVector;*/
	BoxComponent = nullptr;
}

FMeleeInfo::FMeleeInfo(UBoxComponent* BoxComponentIn): BoxComponent(nullptr)
{
	BoxComponent = BoxComponentIn;
}
