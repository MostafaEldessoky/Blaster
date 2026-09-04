// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "Kismet/KismetMathLibrary.h"
#include "Projectiles/Projectile.h"


AProjectileWeapon::AProjectileWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectileWeapon::MultiCastFire_Implementation(FVector_NetQuantize Point)
{
	Super::MultiCastFire_Implementation(Point);
	FRotator StartDirection = UKismetMathLibrary::GetDirectionUnitVector(WeaponMesh->GetSocketLocation(MuzzleSocketName),Point).Rotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass,WeaponMesh->GetSocketLocation(MuzzleSocketName),StartDirection, SpawnParams);
}

