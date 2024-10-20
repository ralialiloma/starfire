// Copyright Epic Games, Inc. All Rights Reserved.

#include "StarFireGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

#define DEFINE_GAMEPLAY_TAG_AUTO(NameSpace, TagName, Comment) \
		STARFIRE_API UE_DEFINE_GAMEPLAY_TAG_COMMENT(TagName, TEXT(#NameSpace "." #TagName), Comment)

namespace Sf_GameplayTags
{
	namespace Animation
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation", "")
		namespace Character
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character", "")
			namespace TP
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.TP", "")
				namespace Montage
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.TP.Montage", "")
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Montage, Fire, "Fire Montage Third Person");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Montage, Melee, "Melee Montage Third Person");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Montage, Reload, "Reload Montage Third Person");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Montage, Aim, "Aim Montage Third Person");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Montage, Equip, "Equip Montage Third Person");
				}
				namespace Blendspace
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.TP.Blendspace", "")
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Blendspace,Walk,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Blendspace,Crouch,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Blendspace,Hip,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Blendspace,Aim,"");
				}
				namespace AnimSequence
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.TP.AnimSequence", "")
					namespace UpperBody
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.TP.AnimSequence.UpperBody", "")
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.UpperBody,Idle,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.UpperBody,Aim,"");
					}
					namespace LowerBody
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.TP.AnimSequence.LowerBody", "")
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.LowerBody,Idle,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.LowerBody,IdleCrouch,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.LowerBody,Fall,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.LowerBody,StandToCrouch,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.LowerBody,CrouchToStand,"");
					}
					namespace FullBody
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.TP.AnimSequence.FullBody", "")
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.FullBody,Death,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.FullBody,Hit,"");
					}
				}
			}
			}
			namespace FP
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.FP", "")
			}
		}
	
	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TagToTest : AllTags)
			{
				if (TagToTest.ToString().Contains(TagString))
				{
					UE_LOG(LogGameplayTags, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, 
					*TagToTest.ToString());
					Tag = TagToTest;
					break;
				}
			}
		}

		return Tag;
	}

	TArray<FGameplayTag> GetAllChildTags(const FGameplayTag& ParentTag)
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
			if (Tag.IsValid() && Tag.MatchesTag(ParentTag)) // Check if the tag is a child of the parent tag
			{
				ChildTags.Add(Tag);
			}
		}

		return ChildTags;
	}
}

