﻿#pragma once
#include "NativeGameplayTags.h"

namespace Sf_GameplayTags 
{
	STARFIRE_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);
	STARFIRE_API TArray<FGameplayTag> GetAllChildTags(const FGameplayTag& ParentTag);
	
	namespace Animation
	{
		STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
		namespace Character
		{
			STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
			namespace TP
			{
				STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
				namespace Montage
				{
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Melee);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Reload);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aim);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Equip);
				}
				namespace Blendspace
				{
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Walk);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Crouch);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Hip);
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aim);
				}
				
				namespace AnimSequence
				{
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
					namespace UpperBody
					{
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Idle);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aim);
					}

					namespace LowerBody
					{
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Idle);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(IdleCrouch);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fall);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StandToCrouch);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CrouchToStand);
					}

					namespace FullBody
					{
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Death);
						STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Hit);
					}
				}
			}
			namespace FP
			{
				STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
			}
		}
	}
}