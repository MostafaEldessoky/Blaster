// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Item/Weapons/ProjectleWeapons/Projectiles/Projectile.h"
#include "Rocket.generated.h"

UCLASS()
class BLASTER_API ARocket : public AProjectile
{
	GENERATED_BODY()

public:
	
	ARocket();
	
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	float DamageRadius{100.0f};
};
