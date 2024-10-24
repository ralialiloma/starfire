#include "Sf_GameplayTagUtil.h"

TArray<FString> FSf_GameplayTagUtil::GetAllChildTagsAsString(const FGameplayTag& ParentTag)
{
	const TArray<FGameplayTag> ALlChildTags = GetAllChildTags(ParentTag);
	return GetGameplayTagsAsString(ALlChildTags);
}

TArray<FGameplayTag> FSf_GameplayTagUtil::GetDirectChildTags(const FGameplayTag& ParentTag)
{
	TArray<FGameplayTag> ChildTags;
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTagContainer AllChildren = Manager.RequestGameplayTagChildren(ParentTag);

	for (const FGameplayTag& ChildTag : AllChildren)
	{
		if (Manager.RequestGameplayTagDirectParent(ChildTag) == ParentTag)  // Check if the child tag's parent is the given parent tag
		{
			ChildTags.Add(ChildTag);  // Add only direct children
		}
	}

	return ChildTags;  // Return the array containing only direct children
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

TArray<FGameplayTag> FSf_GameplayTagUtil::GetAllParentTags(const FGameplayTag& ChildTag)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTagContainer AllTags =  Manager.RequestGameplayTagParents(ChildTag);
	return AllTags.GetGameplayTagArray();
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


