// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGrenade::AGrenade()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMovementComp->bShouldBounce = true;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
	if (!bExplodedOnBounce) GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AGrenade::Explode, ExplosionTime);
}

void AGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (bExplodedOnBounce) GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AGrenade::Explode, ExplosionTime);
}

void AGrenade::Explode()
{
	if (GetOwner()->HasAuthority()) UGameplayStatics::ApplyRadialDamageWithFalloff(this,
    		Damage,
    		Damage/80.f,
    		GetActorLocation(),
    		InnerRadius,
    		OuterRadius,
    		1.f,
    		UDamageType::StaticClass(),
    		TArray<AActor*>(),
    		this,
    		GetOwner()->GetInstigatorController(),
    		ECC_MAX
    		);
	OnDestroyed(GetActorLocation());
}
