// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Item/Weapons/Weapon.h"
#include "HitScanWeapon.generated.h"

class UNiagaraSystem;

UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	
	AHitScanWeapon();
	
	virtual void MultiCastFire_Implementation(FVector_NetQuantize Point) override;
	
	void ComputeHit(FVector_NetQuantize Point);
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	float Damage;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	USoundBase* FireSound;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	UNiagaraSystem* MuzzleFlash;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	bool bRandom;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	float RandRadius;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	int8 NumOfShoots;
	
};
