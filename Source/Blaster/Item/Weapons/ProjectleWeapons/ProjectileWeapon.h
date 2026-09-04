// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Item/Weapons/Weapon.h"
#include "ProjectileWeapon.generated.h"

class AProjectile;

UCLASS()
class BLASTER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	
	AProjectileWeapon();
	
	virtual void MultiCastFire_Implementation(FVector_NetQuantize Point) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	TSubclassOf<AProjectile> ProjectileClass;
	
};
