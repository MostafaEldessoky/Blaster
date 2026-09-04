// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;
class UNiagaraComponent;

UCLASS(Abstract)
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()

public:

	AProjectile();
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:

	virtual void BeginPlay() override;
	
	void OnDestroyed(FVector HitPoint);
	
	EHitReactMontage ComputeHitDirection(FVector ActorForwardVector,FVector HitNormal);
	
	UPROPERTY(VisibleAnywhere,Category = "CPPVars")
	UBoxComponent* CollisionComp;
	
	UPROPERTY(VisibleAnywhere,Category = "CPPVars")
	UNiagaraComponent* NiagaraComp;
	
	UPROPERTY(VisibleAnywhere,Category = "CPPVars")
	UProjectileMovementComponent* ProjectileMovementComp;
	
	UPROPERTY(EditDefaultsOnly,Category = "CPPVars")
	USoundBase* ImpactSound;
	
	UPROPERTY(EditDefaultsOnly,Category = "CPPVars")
	UNiagaraSystem* ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	float Damage = 20.f;
	
};
