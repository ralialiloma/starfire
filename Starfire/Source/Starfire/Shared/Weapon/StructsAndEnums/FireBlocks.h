#pragma once

UENUM(BlueprintType)
enum class EFireBlock: uint8
{
	None = 0 ,
	Reload = 1 UMETA(ToolTip="The weapon is reloading."),
	EmptyClip = 2 UMETA(ToolTip="Empty Clip."),
	NotEnoughAmmo = 3 UMETA(ToolTip="Ammo is too low"),
	Jammed = 4 UMETA(ToolTip="The weapon is jammed."),
	FireCooldown = 5 UMETA(ToolTip="The weapon can fire but is still delaying due to its fire rate."),
	TriggerType = 6 UMETA(ToolTip="The weapon cant fire at the given trigger type."),
	Error = 7 UMETA(ToolTip="Error Case For Unhandled Cases"),
	NoWeapon = 8 UMETA(ToolTip="Mising Weapon"),
	MeleeCooldown = 9 UMETA(ToolTip="The weapon can fire but is still delaying due to its melee cooldown."),
	Incompatible = 10 UMETA(ToolTip="The weapon does not have the ability to melee"),
};

