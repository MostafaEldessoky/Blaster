#pragma once

UENUM()
enum class EHitReactMontage: uint8
{
	FWD,
	BWD,
	LEFT,
	RIGHT
};

UENUM()
enum class EWeaponType: uint8
{
	BULLET,
	ROCKET,
	GRENADE,
	GUN,
	MACHINEGUN,
	SHOTGUN,
	SNIPER
};

UENUM()
enum class EAmmoType: uint8
{
	HEALTH,
	HEAVY,
	SHOTGUN,
	SNIPER,
	MACHINEGUN,
	GRENADE
};

UENUM()
enum class EPuffType: uint8
{
	Heal,
	Jump,
	Speed,
	Shield,
	Ammo,
	Grenade
};

UENUM()
enum class EStateTimer: uint8
{
	WarmUp,
	Match,
	Cooldown
};