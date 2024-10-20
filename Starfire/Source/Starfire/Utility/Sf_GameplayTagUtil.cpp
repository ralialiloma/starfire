#include "Sf_GameplayTagUtil.h"

TArray<FString> FSf_GameplayTagUtil::GetAllChildTagsAsString(const FGameplayTag& ParentTag)
{
	const TArray<FGameplayTag> ALlChildTags = GetAllChildTags(ParentTag);
	return GetGameplayTagsAsString(ALlChildTags);
}

TArray<FGameplayTag> FSf_GameplayTagUtil::GetAllChildTags(const FGameplayTag& ParentTag)
{
	TArray<FGameplayTag> ChildTags;

	// Get the Gameplay Tags Manager
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	// Get all gameplay tags in the system
	FGameplayTagContainer AllTags;
	Manager.RequestAllGameplayTags(AllTags, true);

	// Iterate through all tags and check if they are children of the specified parent tag
	for (const FGameplayTag& Tag : AllTags)
	{
		if (Tag.IsValid() && Tag.MatchesTag(ParentTag) && Tag != ParentTag) // Check if the tag is a child of the parent tag
		{
			ChildTags.Add(Tag);
		}
	}

	return ChildTags;
}

TArray<FString> FSf_GameplayTagUtil::GetGameplayTagsAsString(TArray<FGameplayTag> Tags)
{
	TArray<FString> TagStrings;

	// Convert each gameplay tag to its string representation and add to the array
	for (const FGameplayTag& Tag : Tags)
	{
		TagStrings.Add(Tag.ToString());
	}

	return TagStrings;
}

void FSf_GameplayTagUtil::Test()
{
}
