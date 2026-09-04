// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Item/Weapons/ProjectleWeapons/Projectiles/Projectile.h"
#include "Grenade.generated.h"

UCLASS()
class BLASTER_API AGrenade : public AProjectile
{
	GENERATED_BODY()

public:

	AGrenade();

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	float InnerRadius = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	float OuterRadius = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	float ExplosionTime{2.f};
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	bool bExplodedOnBounce{false};
	
	FTimerHandle ExplosionTimerHandle;
	
	void Explode();
};
