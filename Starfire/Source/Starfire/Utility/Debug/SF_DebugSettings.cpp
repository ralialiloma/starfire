#include "SF_DebugSettings.h"

#include "Starfire/Utility/Sf_FunctionLibrary.h"


#if WITH_EDITOR
void USF_DebugSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	USf_FunctionLibrary::ValidateAndUpdateEnumMap<EDebugVisualType,FGameplayTagContainer>(ShownDebugs);
		USf_FunctionLibrary::ValidateAndUpdateEnumMap<EDebugVisualType,bool>(ShownDebugTypes);
}
#endif

TArray<FGameplayTag> USF_DebugSettings::GetAllDebugTags(const EDebugVisualType DebugType) const
{
	TArray<TMap<EDebugVisualType, FGameplayTagContainer>> AllGameplayTagContainers = GetAllGameplayTagContainers();

	TArray<FGameplayTagContainer> DebugContainers{};
	for (TMap<EDebugVisualType, FGameplayTagContainer> ContainerMap: AllGameplayTagContainers)
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

bool USF_DebugSettings::ShouldShowDebugType (const EDebugVisualType DebugTypeIn) const
{
	if (bHideAllDebugs)
		return false;

	if (!ShownDebugTypes.Contains(DebugTypeIn))
		return true;

	const bool* FoundDebugSettingPtr =  ShownDebugTypes.Find(DebugTypeIn);
	if (FoundDebugSettingPtr == nullptr)
		return true;

	const bool FoundDebugSetting = *FoundDebugSettingPtr;
	return FoundDebugSetting;
}

bool USF_DebugSettings::ShouldDebug(const FGameplayTag DebugTagIn, const EDebugVisualType DebugTypeIn) const
{
	if(bHideAllDebugs)
		return false;

	if (!ShouldShowDebugType(DebugTypeIn))
		return false;
	
	if (!DebugTagIn.MatchesTag(Sf_GameplayTags::Debug::Name))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Only Allowed To Use Subtags of %s for this function"),*Sf_GameplayTags::Debug::Name.GetTag().GetTagName().ToString())
		return false;
	}

	const TArray<FGameplayTag> GameplayTagContainers = GetAllDebugTags(DebugTypeIn);
	return GameplayTagContainers.ContainsByPredicate([&](const FGameplayTag& TagInContainer) {
		return TagInContainer.MatchesTag(DebugTagIn);
	});
}

TArray<TMap<EDebugVisualType, FGameplayTagContainer>> USF_DebugSettings::GetAllGameplayTagContainers() const
{
	return {ShownDebugs};
}