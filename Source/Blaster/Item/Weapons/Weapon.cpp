// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"


#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Character/Controller/BlasterController.h"
#include "Blaster/Character/Controller/UserWidgets/InGameWidget.h"
#include "Case/Case.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


AWeapon::AWeapon()
{

	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	SpereComponent->SetupAttachment(GetRootComponent());
	
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
	
	WeaponMesh->SetIsReplicated(true);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWeapon, WeaponAmmo);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Weapon");
}

void AWeapon::PlayFireMontage(FName MontageSection)
{
	if (!FireAnimation) return;
	WeaponMesh->GetAnimInstance()->Montage_Play(FireAnimation);
	WeaponMesh->GetAnimInstance()->Montage_JumpToSection(MontageSection, FireAnimation);
}

void AWeapon::ServerFire_Implementation(FVector_NetQuantize Point)
{
	if (WeaponAmmo <= 0 )
	{
		if (Cast<ABlasterCharacter>(GetOwner())->GetCarriedAmmo() <= 0) 
		{
			return;
		}
		Cast<ABlasterCharacter>(GetOwner())->ServerReload();
	}
	if(!bCurrentFiring)
	{
		MultiCastFire(Point);
		bCurrentFiring = true;
		WeaponAmmo = FMath::Clamp(WeaponAmmo - 1, 0, MaxWeaponAmmo);
		UpdateAmmoWidget(WeaponAmmo);
		GetWorld()->GetTimerManager().SetTimer(FireTimer,this,&AWeapon::AutomaticFire,FireRate);
	}
}

void AWeapon::AutomaticFire()
{
	bCurrentFiring = false;
}

void AWeapon::UpdateAmmoWidget(int8 weaponAmmo)
{
	if (GetOwner() && Cast<ABlasterCharacter>(GetOwner())->IsLocallyControlled()) Cast<ABlasterController>(Cast<ABlasterCharacter>(GetOwner())->GetController())->GetOwnedWidget()->SetWeaponAmmo(weaponAmmo);
}

void AWeapon::OnRep_WeaponAmmo()
{
	UpdateAmmoWidget(WeaponAmmo);
}

void AWeapon::MultiCastFire_Implementation(FVector_NetQuantize Point)
{
	auto temp = Cast<ABlasterCharacter>(GetOwner());
	if (!temp) return;
	if (temp->bAiming)
	{
		PlayFireMontage(AimFireMontageSection);
	}
	else
	{
		PlayFireMontage(HipFireMontageSection);
	}
	GetWorld()->SpawnActor<ACase>(CaseClass,WeaponMesh->GetSocketLocation(CaseSocketName), WeaponMesh->GetSocketRotation(CaseSocketName));
}

void AWeapon::PickUp(ABlasterCharacter* BlasterPtr)
{
	Super::PickUp(BlasterPtr);
	WeaponMesh->SetSimulatePhysics(false);
    WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToComponent(BlasterPtr->GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,BlasterPtr->WeaponSocketName);
	UpdateAmmoWidget(WeaponAmmo);
}

void AWeapon::Drop()
{
	UpdateAmmoWidget(0);
	Super::Drop();
	GetRootComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	WeaponMesh->SetSimulatePhysics(true);
	WeaponMesh->SetEnableGravity(true);
}


