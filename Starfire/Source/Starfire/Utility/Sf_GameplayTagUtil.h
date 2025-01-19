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
	static TArray<FGameplayTag> GetDirectChildTags(const FGameplayTag& ParentTag);
	static TArray<FGameplayTag> GetAllParentTags(const FGameplayTag& ChildTag);
	static TArray<FString> GetGameplayTagsAsString(TArray<FGameplayTag> Tags);

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
			Map.Add(Tag, nullptr);
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

	KeysToRemove.Empty();
	// Iterate over all keys in the map
	for (const auto& OuterKvp : Map)
	{
		const FGameplayTag& OuterTag = OuterKvp.Key;

		// Check if this tag is a parent of any other tag in the map
		for (const auto& InnerKvp : Map)
		{
			const FGameplayTag& InnerTag = InnerKvp.Key;
			if (OuterTag != InnerTag && OuterTag.MatchesTag(InnerTag))
			{
				// If OuterTag is a parent of InnerTag, mark it for removal
				KeysToRemove.AddUnique(InnerTag);
				break; // No need to check further for this tag
			}
		}
	}

	// Remove parent tags that were marked
	for (const FGameplayTag& Key : KeysToRemove)
	{
		Map.Remove(Key);
	}

	
}