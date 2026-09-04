// Fill out your copyright notice in the Description page of Project Settings.


#include "Rocket.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ARocket::ARocket()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetOwner()->HasAuthority()) UGameplayStatics::ApplyRadialDamage(this,
		Damage, 
		Hit.ImpactPoint, 
		DamageRadius,
		UDamageType::StaticClass(), 
		TArray<AActor*>(), 
		this,
		GetOwner()->GetInstigatorController(),
		true,
		ECC_MAX);
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
