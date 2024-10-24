#include "DebugSettings.h"


void UDebugSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EDebugType,FGameplayTagContainer>(TP);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EDebugType,FGameplayTagContainer>(Weapon);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EDebugType,FGameplayTagContainer>(FP);
}

TArray<FGameplayTag> UDebugSettings::GetAllDebugTags(const EDebugType DebugType) const
{
	TArray<TMap<EDebugType, FGameplayTagContainer>> AllGameplayTagContainers = GetAllGameplayTagContainers();

	TArray<FGameplayTagContainer> DebugContainers{};
	for (TMap<EDebugType, FGameplayTagContainer> ContainerMap: AllGameplayTagContainers)
	{
		const FGameplayTagContainer* FoundContainerPtr =  ContainerMap.Find(DebugType);
		if (FoundContainerPtr == nullptr)
			continue;
		DebugContainers.Add(*FoundContainerPtr);
	}

	TArray<FGameplayTag> AllTags{};
	for (FGameplayTagContainer Container: DebugContainers)
	{
		AllTags.Append(Container.GetGameplayTagArray());
	}
	
	return AllTags;
}

bool UDebugSettings::ShouldDebug(const FGameplayTag DebugTagIn, const EDebugType DebugTypeIn) const
{
	if(bHideAllDebugs)
		return false;
	
	if (!DebugTagIn.MatchesTag(Sf_GameplayTags::Debug::Name))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Only Allowed To Use Subtags of %s for this function"),*Sf_GameplayTags::Debug::Name.GetTag().GetTagName().ToString())
		return false;
	}

	TArray<FGameplayTag> GameplayTagContainers = GetAllDebugTags(DebugTypeIn);
	for (FGameplayTag TagInContainer: GameplayTagContainers)
	{
		if (TagInContainer.MatchesTagExact(DebugTagIn))
		{
			return true;
		}
	}
	return false;
}

TArray<TMap<EDebugType, FGameplayTagContainer>> UDebugSettings::GetAllGameplayTagContainers() const
{
	return {TP,Weapon,FP};
}