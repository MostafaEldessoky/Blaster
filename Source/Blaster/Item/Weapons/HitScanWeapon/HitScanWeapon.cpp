// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AHitScanWeapon::AHitScanWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHitScanWeapon::MultiCastFire_Implementation(FVector_NetQuantize Point)
{
	Super::MultiCastFire_Implementation(Point);
	ComputeHit(Point);
}

void AHitScanWeapon::ComputeHit(FVector_NetQuantize EndPoint)
{
	FVector ImpactPoint;
	auto StartPoint = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	auto temp = EndPoint + ((EndPoint-StartPoint)/(EndPoint-StartPoint).Size())*RandRadius;
	for (int i = 0; i < NumOfShoots; i++)
	{
		if (bRandom)
		{
			temp = EndPoint + UKismetMathLibrary::RandomUnitVector() * UKismetMathLibrary::RandomFloatInRange(0,RandRadius);
			temp += ((temp-StartPoint)/(temp-StartPoint).Size())*RandRadius;
		}
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, StartPoint, temp, ECC_Visibility);
		if (!Hit.bBlockingHit) ImpactPoint = temp;
		else ImpactPoint = Hit.ImpactPoint;
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, ImpactPoint);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MuzzleFlash, ImpactPoint);
		if (GetOwner()->HasAuthority() && Hit.bBlockingHit && Hit.GetActor()->ActorHasTag("Player")) 
			UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, ImpactPoint, Hit, 
				GetOwner()->GetInstigatorController(), this, UDamageType::StaticClass());
	}
}

