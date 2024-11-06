// Copyright Epic Games, Inc. All Rights Reserved.

#include "StarFireGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

#define DEFINE_GAMEPLAY_TAG_AUTO(NameSpace, TagName, Comment) \
		STARFIRE_API UE_DEFINE_GAMEPLAY_TAG_COMMENT(TagName, TEXT(#NameSpace "." #TagName), Comment)

namespace Sf_GameplayTags
{
	namespace Gameplay
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay", "")
		namespace DamageType
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.DamageType", "")
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.DamageType,Melee,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.DamageType,Fire,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.DamageType,Explosion,"");
		}
		namespace Resource
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.Resource", "")
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.Resource,Environment,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.Resource,Enemy,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.Resource,ObjectiveItem,"");

		}
		namespace Weapon
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.Weapon", "")
			STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
			namespace Action
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.Weapon.Action", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.Weapon.Action, ActivateWeapon, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.Weapon.Action, AddWeapon, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.Weapon.Action, RemoveWeapon, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.Weapon.Action, EquipWeapon, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.Weapon.Action, UnequipWeapon, "");
			}
		}
	}
	namespace Debug
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug", "")
		namespace TP
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.TP", "")
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP, Controller, "");
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP, Pawn, "");
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP, MoveTargetLocation, "");
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP,RandomRoamLocation,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP, MeleeRangeCheck, "");
			namespace EQS
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.TP.EQS", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.EQS, CoverLocation, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.EQS, PeakLocation, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.EQS, TetherPointGen, "");
			}
			namespace CharacterFeatures
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.TP.CharacterFeatures", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.CharacterFeatures, Combat, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.CharacterFeatures, Cover, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.CharacterFeatures, Locomotion, "");
			}
		}
		namespace FP
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.FP", "")
			namespace Movement
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.FP.Movement", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.FP.Movement, Mantle, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.FP.Movement, Wallrun, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.FP.Movement, Dash, "");
			}
			namespace HSM
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.FP.HSM", "")
			}
		}
		namespace Weapon
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.Weapon", "")
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.Weapon, FireBlocks, "");
		}
		namespace Interactables
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.Interactables", "")
		}
		namespace Inventory
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.Inventory", "");
		}

	}
	namespace Animation
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation", "")
		namespace AnimNotify
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.AnimNotify", "")
			DEFINE_GAMEPLAY_TAG_AUTO(Animation.AnimNotify, ThrowKnife, "");
			DEFINE_GAMEPLAY_TAG_AUTO(Animation.AnimNotify, HitMelee, "");
		}
		namespace Weapon
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Weapon", "")
			namespace Event
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Weapon.Event", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Event, Fire, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Event, Reload, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Event, Aim, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Event, Equip, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Event, Melee, "");
			}	
		}
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
			namespace FP
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.FP", "");
				namespace Montage
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.FP.Montage", "");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.Montage,Fire,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.Montage,Death,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.Montage,Melee,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.Montage,Reload,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.Montage,Equip,"");
				}
				namespace Blendspace
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.FP.Blendspace", "");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.Blendspace,Walk,"");
				}
				namespace AnimSequence
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.FP.AnimSequence", "");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.AnimSequence,Idle,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.AnimSequence,Jump,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.AnimSequence,Falling,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.AnimSequence,Land,"");
					}
				}
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

