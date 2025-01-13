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
		namespace ActionLogger
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger", "")
			namespace PortalSystem
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.PortalSystem", "")
				namespace Pillar
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.PortalSystem.Pillar", "")
					DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.PortalSystem.Pillar,Break,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.PortalSystem.Pillar,Restore,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.PortalSystem.Pillar,RestoreProgress,"");
				}
			}
			namespace FP
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP", "")
				namespace Movement
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Movement", "")
					DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Movement,Jump,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Movement,Dash,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Movement,Mantle,"");
					namespace Sprint
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Movement.Sprint", "")
						DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Movement.Sprint,Start,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Movement.Sprint,End,"");
					}
					namespace WallRun
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Movement.Wallrun", "")
						DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Movement.Wallrun,Start,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Movement.Wallrun,Jump,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Movement.Wallrun,End,"");
					}
				}
				namespace Equipment
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Equipment", "")
					namespace Rifle
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Equipment.Rifle", "")
						namespace Fire
						{
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Equipment.Rifle.Fire", "")
							DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Rifle.Fire,Start,"");
							DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Rifle.Fire,Tick,"");
							DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Rifle.Fire,End,"");
							DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Rifle.Fire,Kill,"");
						}
						namespace Reload
						{
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Equipment.Rifle.Fire.Reload", "")
							DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Rifle.Reload,Start,"");
							DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Rifle.Reload,End,"");
						}
					}
					namespace Grenade
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Equipment.Grenade", "")
						DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Grenade,Craft,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Grenade,Explode,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Grenade,Kill,"");
						namespace Throw
						{
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.ActionLogger.FP.Equipment.Grenade.Throw", "")
							DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Grenade.Throw,Start,"");
							DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.ActionLogger.FP.Equipment.Grenade.Throw,End,"");
						}
					}
				}
			}
		}
		namespace PatrolAreaMarkerTypes
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.PatrolAreaMarkerTypes", "")
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.PatrolAreaMarkerTypes,Peak,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.PatrolAreaMarkerTypes,Roam,"");
			namespace Cover
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.PatrolAreaMarkerTypes.Cover", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.PatrolAreaMarkerTypes.Cover,Low,"");
				DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.PatrolAreaMarkerTypes.Cover,High,"");
			}
		}
		
		namespace DamageType
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.DamageType", "")
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.DamageType,Melee,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.DamageType,FirePlayer,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.DamageType,FireEnemy,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.DamageType,Explosion,"");
		}
		namespace PlayState
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Gameplay.PlayState", "")
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.PlayState,Tutorial,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.PlayState,Arena,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Gameplay.PlayState,End,"");
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
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP, DynamicMoveTarget, "");
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP,RandomRoamLocation,"");
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP, MeleeRangeCheck, "");
			namespace PatrolArea
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.TP.PatrolArea", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.PatrolArea, PatrolMarker, "");
			}
			namespace EQS
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.TP.EQS", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.EQS, Scheduler, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.EQS, CoverLocation, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.EQS, PeakLocation, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.EQS, TetherPointGen, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Debug.TP.EQS, CloseToPlayerLocations, "");
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
		namespace Core
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.Core", "");
			DEFINE_GAMEPLAY_TAG_AUTO(Debug.Core, GameState, "");
		}
		namespace Spawning
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Debug.Spawning", "");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resources, "Debug.Spawning.Resources", "");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemies, "Debug.Spawning.Enemies", "");
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

			namespace Montage
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Weapon.Montage", "")
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Montage, Fire, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Montage, Melee, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Montage, Reload, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Montage, Aim, "");
				DEFINE_GAMEPLAY_TAG_AUTO(Animation.Weapon.Montage, Equip, "");
			}
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
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.Montage, Fire, "FirePlayer Montage Third Person");
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
					}
					namespace LowerBody
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Animation.Character.TP.AnimSequence.LowerBody", "")
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.LowerBody,Idle,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.TP.AnimSequence.LowerBody,IdleCrouch,"");
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
					DEFINE_GAMEPLAY_TAG_AUTO(Animation.Character.FP.Montage,Unequip, "");
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
	namespace Effects
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects", "");
		namespace FX
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX", "");
			namespace SoundsFX
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX", "");
				namespace FP
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Respawn, "Effects.FX.SoundFX.FP.Respawn", "");
					namespace Health
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Health", "");
						DEFINE_GAMEPLAY_TAG_AUTO(Effects.FX.SoundFX.FP.Health,Low,"");
						namespace Heal
						{
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Health.Heal", "");
							DEFINE_GAMEPLAY_TAG_AUTO(Effects.FX.SoundFX.FP.Health,Start,"");
							DEFINE_GAMEPLAY_TAG_AUTO(Effects.FX.SoundFX.FP.Health,Full,"");
						}
					}
					namespace Movement
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Movement", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Jump, "Effects.FX.SoundFX.FP.Movement.Jump", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Footstep, "Effects.FX.SoundFX.FP.Movement.Footstep", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dash, "Effects.FX.SoundFX.FP.Movement.Dash", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mantle, "Effects.FX.SoundFX.FP.Movement.Mantle", "");
						namespace WallRun
						{
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Respawn, "Effects.FX.SoundFX.FP.Movement.WallRun", "");
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Start, "Effects.FX.SoundFX.FP.Movement.WallRun.Start", "");
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Step, "Effects.FX.SoundFX.FP.Movement.WallRun.Step", "");
						}
					}
					namespace Equipment
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Equipment", "");
						namespace Rifle
						{
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Equipment.Rifle", "");
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fire, "Effects.FX.SoundFX.FP.Equipment.Rifle.Fire", "");
							namespace HitTarget
							{
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Equipment.Rifle.HitTarget", "");
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(Pillar, "Effects.FX.SoundFX.FP.Equipment.Rifle.HitTarget.Pillar", "");
							}
							namespace Reload
							{
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Equipment.Rifle.Reload", "");
								namespace SafetyLock
								{
									UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Equipment.Rifle.Reload.SafetyLock", "");
									namespace SafetyLock
									{
										UE_DEFINE_GAMEPLAY_TAG_COMMENT(Open, "Effects.FX.SoundFX.FP.Equipment.Rifle.Reload.SafetyLock.Open", "");
										UE_DEFINE_GAMEPLAY_TAG_COMMENT(Close, "Effects.FX.SoundFX.FP.Equipment.Rifle.Reload.SafetyLock.Close", "");
									}
								}
								namespace AmmoPack
								{
									UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Equipment.Rifle.Reload.AmmoPack", "");
									UE_DEFINE_GAMEPLAY_TAG_COMMENT(RemoveFromShaft, "Effects.FX.SoundFX.FP.Equipment.Rifle.Reload.AmmoPack.RemoveFromShaft", "");
									UE_DEFINE_GAMEPLAY_TAG_COMMENT(FailedAttemtToAddToShaft, "Effects.FX.SoundFX.FP.Equipment.Rifle.Reload.AmmoPack.FailedAttemtToAddToShaft", "");
									UE_DEFINE_GAMEPLAY_TAG_COMMENT(AddToShaft, "Effects.FX.SoundFX.FP.Equipment.Rifle.Reload.AmmoPack.AddToShaft", "");
								}
							}
						}
					namespace Grenade
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.SoundFX.FP.Equipment.Grenade", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Contact, "Effects.FX.SoundFX.FP.Equipment.Grenade.Contact", "");
					}
				}
			}
			}
			namespace VisualFX
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.FX.VisualFX", "");
			}
		}
		namespace Messages
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages", "");
			namespace PortalSystem
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.PortalSystem", "");
				namespace Pillar
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.PortalSystem.Pillar", "");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.PortalSystem.Pillar,Break,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.PortalSystem.Pillar,Restore,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.PortalSystem.Pillar,RestoreProgress,"");
					namespace Move
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.PortalSystem.Pillar.Move", "");
						DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.PortalSystem.Pillar.Move,Up,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.PortalSystem.Pillar.Move,Down,"");
					}
				}
				namespace Portal
				{
					STARFIRE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Name);
				}
			}
			namespace TP
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.TP", "");
				DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.TP,Death,"");
				namespace Equipment
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.TP.Equipment", "");
					namespace Rifle
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.TP.Equipment.Rifle", "");
						DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.TP.Equipment.Rifle,Melee,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.TP.Equipment.Rifle,Fire,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.TP.Equipment.Rifle,Reload,"");
					}
				}
				namespace Movement
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.TP.Movement", "");
					namespace Step
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.TP.Movement.Step", "");
						DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.TP.Movement.Step,Walk,"");
						DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.TP.Movement.Step,Sprint,"");
					}
				}
			}
			namespace Resources
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.Resources", "");
				namespace Crystal
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.Resources.Crystal", "");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.Resources.Crystal,Break,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.Resources.Crystal,Pickup,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.Resources.Crystal,Full,"");
				}
				namespace PowerCore
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.Resources.PowerCore", "");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.Resources.PowerCore,Pickup,"");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.Resources.PowerCore,Full,"");
				}
				namespace Grenade
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.Resources.Grenade", "");
					DEFINE_GAMEPLAY_TAG_AUTO(Effects.Messages.Resources.Grenade,Craft,"");
				}
			}
			namespace FP
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Respawn, "Effects.Messages.FP.Respawn", "");
				namespace Movement
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.FP.Movement", "");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Jump, "Effects.Messages.FP.Movement.Jump", "");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Footstep, "Effects.Messages.FP.Movement.Footstep", "");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dash, "Effects.Messages.FP.Movement.Dash", "");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mantle, "Effects.Messages.FP.Movement.Mantle", "");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Land, "Effects.Messages.FP.Movement.Land", "");
					namespace WallRun
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.FP.Movement.WallRun", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Start, "Effects.Messages.FP.Movement.WallRun.Start", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Step, "Effects.Messages.FP.Movement.WallRun.Step", "");
					}
				}
				namespace Equipment
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.FP.Equipment", "");
					namespace Rifle
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.FP.Equipment.Rifle", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fire, "Effects.Messages.FP.Equipment.Rifle.Fire", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(EmptyClipFireAttempt, "Effects.Messages.FP.Equipment.Rifle.EmptyClipFireAttempt", "");
						namespace Reload
						{
							UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.FP.Equipment.Rifle.Reload", "");
							namespace SafetyLock
							{
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.FP.Equipment.Rifle.Reload.SafetyLock", "");
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(Open, "Effects.Messages.FP.Equipment.Rifle.Reload.SafetyLock.Open", "");
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(Close, "Effects.Messages.FP.Equipment.Rifle.Reload.SafetyLock.Close", "");
							}
							namespace AmmoPack
							{
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.FP.Equipment.Rifle.Reload.AmmoPack", "");
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(RemoveFromShaft, "Effects.Messages.FP.Equipment.Rifle.Reload.AmmoPack.RemoveFromShaft", "");
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(FailedAttemtToAddToShaft, "Effects.Messages.FP.Equipment.Rifle.Reload.AmmoPack.FailedAttemtToAddToShaft", "");
								UE_DEFINE_GAMEPLAY_TAG_COMMENT(AddToShaft, "Effects.Messages.FP.Equipment.Rifle.Reload.AmmoPack.AddToShaft", "");
							}
						}
					}
					namespace Grenade
					{
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Name, "Effects.Messages.FP.Equipment.Grenade", "");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Contact, "Effects.Messages.FP.Equipment.Grenade.Contact", "");
					}
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

