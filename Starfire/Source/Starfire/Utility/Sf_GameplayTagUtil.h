#pragma once
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
#include "../StarFireGameplayTags.h"
#include "Sf_GameplayTagUtil.generated.h"

USTRUCT()
struct FSf_GameplayTagUtil
{
	GENERATED_BODY()
public:
	static TArray<FString> GetAllChildTagsAsString(const FGameplayTag& ParentTag);
	static TArray<FGameplayTag> GetAllChildTags(const FGameplayTag& ParentTag);
	static TArray<FString> GetGameplayTagsAsString(TArray<FGameplayTag> Tags);

	static void Test();

	template <typename ValueType>
	static void ValidateAndUpdateGameplayTagMap(TMap<FGameplayTag, ValueType>& Map, const FGameplayTag& ParentTag);
};

template <typename ValueType>
void FSf_GameplayTagUtil::ValidateAndUpdateGameplayTagMap(TMap<FGameplayTag, ValueType>& Map, const FGameplayTag& ParentTag)
{
	TArray<FGameplayTag> ChildTags = GetAllChildTags(ParentTag);

	// Import New Tags
	for (const FGameplayTag& Tag : ChildTags)
	{
		if (!Map.Contains(Tag))
		{
			Map.Add(Tag, ValueType());
		}
	}

	// Collect Invalid Tags for Removal
	TArray<FGameplayTag> KeysToRemove;

	for (const auto& Kvp : Map)
	{
		if (!ChildTags.Contains(Kvp.Key))
		{
			KeysToRemove.Add(Kvp.Key);
		}
	}

	// Remove invalid tags outside of the iteration
	for (const auto& Key : KeysToRemove)
	{
		Map.Remove(Key);
	}
}