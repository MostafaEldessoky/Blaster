// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Blaster/Item/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	
	CollisionComp->SetCollisionObjectType( ECollisionChannel::ECC_WorldDynamic);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CollisionComp->SetGenerateOverlapEvents(true);
	
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->InitialSpeed = 20000.f;
	ProjectileMovementComp->MaxSpeed = 20000.f;
	
}

EHitReactMontage AProjectile::ComputeHitDirection(FVector ActorForwardVector,FVector HitNormal)
{
	auto temp1 = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Vector_CosineAngle2D(ActorForwardVector,HitNormal));
	auto temp2 = UKismetMathLibrary::Cross_VectorVector(ActorForwardVector,HitNormal).Z;
	if (temp1 > 0.f && temp1 <= 45.f)
	{
		return EHitReactMontage::FWD;
	}
	if(temp1 > 45.f && temp1 <= 135.f && temp2 > 0.f)
	{
		return EHitReactMontage::LEFT;
	}
	if(temp1 > 45.f && temp1 <= 135.f && temp2 < 0.f)
	{
		return EHitReactMontage::RIGHT;
	}
	return EHitReactMontage::BWD;

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag(FName("Player"))) Cast<ABlasterCharacter>(OtherActor)->PlayHitReactMontage(
			 ComputeHitDirection(OtherActor->GetActorForwardVector(),
				 Hit.ImpactPoint -  OtherActor->GetActorLocation()),Cast<ABlasterCharacter>(GetOwner())->EquippedWeapon->WeaponType);
	OnDestroyed(Hit.ImpactPoint);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->IgnoreActorWhenMoving(GetOwner(),true);
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnDestroyed(FVector HitPoint)
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,HitPoint);
 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,HitPoint);
	Destroy();
}
