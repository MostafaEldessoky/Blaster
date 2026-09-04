// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/GameplayStatics.h"


ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{

	if (GetOwner()->HasAuthority() && OtherActor->ActorHasTag(FName("Player")))
		UGameplayStatics::ApplyDamage(OtherActor,
		   Damage,
		   GetOwner()->GetInstigatorController(),
		   this,
		   UDamageType::StaticClass());
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

