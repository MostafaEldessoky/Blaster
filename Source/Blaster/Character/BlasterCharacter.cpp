// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"

#include "Blaster/GameMode/MainGameMode.h"
#include "Blaster/Item/Item.h"
#include "Blaster/Item/Puffs/Puff.h"
#include "Blaster/Item/Weapons/Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/BlasterController.h"
#include "Controller/UserWidgets/InGameWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "PlayerState/BlasterState.h"

ABlasterController* ABlasterCharacter::GetBlasterController()
{
	BlasterController = BlasterController == nullptr ? Cast<ABlasterController>(GetController()) : BlasterController;
	return BlasterController;
}

UInGameWidget* ABlasterCharacter::GetBlasterWidget()
{
	if (IsLocallyControlled()) BlasterWidget = BlasterWidget == nullptr ? GetBlasterController()->GetOwnedWidget() : BlasterWidget;
	return BlasterWidget;
}

AMainGameMode* ABlasterCharacter::GetMainGameMode()
{
	if (HasAuthority()) MainGameMode = MainGameMode == nullptr ? GetWorld()->GetAuthGameMode<AMainGameMode>() : MainGameMode;
	return MainGameMode;
}

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetMesh()->SetRelativeLocation(FVector(0,0,-1 * GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,540,0);
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 50.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	GetCharacterMovement()->GroundFriction = 8.f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->SetCrouchedHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/2);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.f;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->SetRelativeLocation(FVector(0,0,GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 20.f));
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	DesolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DesolveTimeline"));
	
	bNetLoadOnClient = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Player"));
	
	DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInstance,this);
}

void ABlasterCharacter::Tick(float deltatime)
{
	Super::Tick(deltatime);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABlasterCharacter,Health);
	DOREPLIFETIME(ABlasterCharacter,XSpeed);
	DOREPLIFETIME(ABlasterCharacter,YSpeed);
	DOREPLIFETIME(ABlasterCharacter,XYAngle);
	DOREPLIFETIME(ABlasterCharacter,XZAngle);
	DOREPLIFETIME(ABlasterCharacter,bAiming);
	DOREPLIFETIME(ABlasterCharacter,EquippedWeapon);
	DOREPLIFETIME(ABlasterCharacter,CarriedAmmo);
	DOREPLIFETIME_CONDITION(ABlasterCharacter,OverlappingItem,COND_OwnerOnly);
}

void ABlasterCharacter::OnDeath()
{
	if(DeathMontage && GetMesh() && GetMesh()->GetAnimInstance())
		GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
	GetMesh()->SetMaterial(0,DynamicMaterialInstance);
	startDesolveTimeline();
 	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
 	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
 	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Ignore);
 	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Ignore);
 	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
 	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
 	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Ignore);
 	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Ignore);
}

void ABlasterCharacter::DesolveTimelineFloat(float value)
{
	if (DynamicMaterialInstance) 
        		DynamicMaterialInstance->SetScalarParameterValue(
        			FName("Param"),
        			value);
}

void ABlasterCharacter::startDesolveTimeline()
{
	OnTimelinefloat.BindDynamic(this,&ABlasterCharacter::DesolveTimelineFloat);
	if (DesolveCurve.IsValid())
	{
		DesolveTimeline->AddInterpFloat(DesolveCurve.Get(),OnTimelinefloat);
		DesolveTimeline->Play();
	}
}

float ABlasterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                    class AController* EventInstigator, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - DamageAmount,0.f,100.f);
	
	if (GetBlasterWidget()) GetBlasterWidget()->SetHealthBar(Health,100.f);
	if (!(Health <= 0.f)) return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (EquippedWeapon) EquippedWeapon->Drop();
	EquippedWeapon = nullptr;
	if (EventInstigator && Cast<ABlasterController>(EventInstigator) != BlasterController && EventInstigator->GetPlayerState<ABlasterState>()) 
		EventInstigator->GetPlayerState<ABlasterState>()->GotKill();
	if (!bIsDead && GetBlasterController() && GetBlasterController()->GetPlayerState<ABlasterState>()) 
		GetBlasterController()->GetPlayerState<ABlasterState>()->GotDeath();
	if (!bIsDead) OnDeath();
	if (!bIsDead) GetMainGameMode()->Elim(this);
	bIsDead = true;
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABlasterCharacter::OnRep_Health()
{
	if (GetBlasterWidget()) GetBlasterWidget()->SetHealthBar(Health,100.f);
	if (!(Health <= 0.f)) return;
	OnDeath();
	if (GetBlasterWidget()) GetBlasterWidget()->SetWeaponAmmo(0);
}

void ABlasterCharacter::Overlapping(AItem* Item,bool bIsOverlapping)
{
	if (bIsOverlapping)
	{
		OverlappingItem = Item;
		if (IsLocallyControlled())
		{
			GetBlasterWidget()->SetPickUpMassage(Item->PickUpMassage);
			GetBlasterWidget()->PickUpMassageVisibility(true);
		}
	}
	else
	{
		OverlappingItem = nullptr;
		if (IsLocallyControlled())
		{
			GetBlasterWidget()->PickUpMassageVisibility(false);
		}
	}
}

void ABlasterCharacter::OnRep_EquippedWeapon()
{
	if (!IsLocallyControlled()) return;
	if (EquippedWeapon && GetBlasterWidget())  GetBlasterWidget()->SetWeaponAmmo(EquippedWeapon->WeaponAmmo);
}

void ABlasterCharacter::OnRep_OverlappingItem(AItem* OverlappedItem)
{
	if (!OverlappedItem)
	{
		GetBlasterWidget()->PickUpMassageVisibility(true);
		GetBlasterWidget()->SetPickUpMassage(OverlappingItem->PickUpMassage);
	}
	else
	{
		GetBlasterWidget()->PickUpMassageVisibility(false);
	}
} 

void ABlasterCharacter::ServerPickUp_Implementation()
{
	if (!OverlappingItem) return;
	if (OverlappingItem->ActorHasTag("PuffHeal")) return;
	if (OverlappingItem->ActorHasTag("PuffJump")) return;
	if (OverlappingItem->ActorHasTag("PuffSpeed")) return;
	if (OverlappingItem->ActorHasTag("PuffShield")) return;
	if (OverlappingItem->ActorHasTag("PuffAmmo"))
	{
		SetCarriedAmmo(CarriedAmmo + Cast<APuff>(OverlappingItem)->Amount);
		if(IsLocallyControlled() && GetBlasterWidget()) GetBlasterWidget()->SetBlasterAmmo(CarriedAmmo + Cast<APuff>(OverlappingItem)->Amount);
		Cast<APuff>(OverlappingItem)->PickUp(this);
		return;
	}
	if (EquippedWeapon)
	{
		auto temp = Cast<AWeapon>(OverlappingItem);
		EquippedWeapon->Drop();
		EquippedWeapon = temp; 
		temp->PickUp(this); 
	}
	else
	{
		EquippedWeapon = Cast<AWeapon>(OverlappingItem); 
		OverlappingItem->PickUp(this);
	}
	
}

void ABlasterCharacter::Zoom(bool bIsZooming)
{
		CameraComponent->FieldOfView = bIsZooming ? EquippedWeapon->AimingFieldOfView : 90;
}

void ABlasterCharacter::UpdateAnimations(FRotator NewRotation)
{
	if (!IsLocallyControlled()) return;
	ServerUpdateAnimations(NewRotation);
}

void ABlasterCharacter::ServerUpdateAnimations_Implementation(FRotator NewRotation)
{
	XSpeed = UKismetMathLibrary::ProjectVectorOnToVector(GetCharacterMovement()->Velocity,
		FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::Y)).Size();
	auto temp1 = GetCharacterMovement()->Velocity;
	temp1.Normalize();
	auto CosTheta1 = UKismetMathLibrary::Dot_VectorVector(
		FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::Y),
		temp1);
	if (CosTheta1 < 0) XSpeed *= -1;
	
	YSpeed = UKismetMathLibrary::ProjectVectorOnToVector(GetCharacterMovement()->Velocity,
		FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::X)).Size();
	auto temp2 = GetCharacterMovement()->Velocity;
	temp2.Normalize();
	auto CosTheta2 = UKismetMathLibrary::Dot_VectorVector(
		FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::X),
		temp2);
	if (CosTheta2 < 0) YSpeed *= -1;
	
	XYAngle = UKismetMathLibrary::DegAcos(
		UKismetMathLibrary::Vector_CosineAngle2D(
			GetActorForwardVector(),
			FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::X)));
		
	if (UKismetMathLibrary::CrossProduct2D(FVector2D(GetActorForwardVector().X,
		GetActorForwardVector().Y),
		FVector2D(FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::X).X,
		FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::X).Y)) < 0)
		XYAngle *= -1;
		
	XZAngle = NewRotation.Pitch;
	
	if (XZAngle > 90) XZAngle -= 360 ;
}

void ABlasterCharacter::Strafing(FRotator NewRotation)
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	auto Theta = UKismetMathLibrary::DegAcos(
			UKismetMathLibrary::Vector_CosineAngle2D(
				GetActorForwardVector(),
				FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::X)));
		
	if (UKismetMathLibrary::CrossProduct2D(FVector2D(GetActorForwardVector().X,
		GetActorForwardVector().Y),
		FVector2D(FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::X).X,
		FRotationMatrix(FRotator(0,NewRotation.Yaw,0)).GetUnitAxis(EAxis::X).Y)) < 0)
		Theta *= -1;
	
	AddActorWorldRotation(FRotator(0,Theta,0));
}

void ABlasterCharacter::ServerStrafing_Implementation(FRotator NewRotation)
{
	Strafing(NewRotation);
}

void ABlasterCharacter::ServerAim_Implementation(bool bISAiming)
{	
	bAiming = bISAiming;
}

void ABlasterCharacter::PlayHitReactMontage(EHitReactMontage HitReactMontageName,EWeaponType ProjectileType)
{
	switch (ProjectileType)
	{
	case EWeaponType::BULLET:
		
		GetMesh()->GetAnimInstance()->Montage_Play(HitReactMontage[EWeaponType::BULLET]);
		switch (HitReactMontageName)
		{
		case EHitReactMontage::FWD:
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(HitReactMontageSectionName_FWD);
			break;
		case EHitReactMontage::BWD:
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(HitReactMontageSectionName_BWD);
			break;
		case EHitReactMontage::LEFT:
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(HitReactMontageSectionName_LEFT);
			break;
		case EHitReactMontage::RIGHT:
			GetMesh()->GetAnimInstance()->Montage_JumpToSection(HitReactMontageSectionName_RIGHT);
			break;
		}
		
		break;
		
	case EWeaponType::ROCKET:
		
		break;
		
	case EWeaponType::GRENADE:
		
		break;
		
	}
}

int8 ABlasterCharacter::GetCarriedAmmo()
{
	if (!EquippedWeapon) return 0;
	return CarriedAmmo;
}

void ABlasterCharacter::SetCarriedAmmo(int8 Ammo)
{
	CarriedAmmo = Ammo;
}

void ABlasterCharacter::OnRep_CarriedAmmo()
{
	GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
	if (IsLocallyControlled())	GetBlasterWidget()->SetBlasterAmmo(CarriedAmmo);
}

void ABlasterCharacter::ServerReload_Implementation()
{
	if (!EquippedWeapon) return;
	if (EquippedWeapon->WeaponAmmo == EquippedWeapon->MaxWeaponAmmo) return;
	auto temp = FMath::Clamp(GetCarriedAmmo() - (EquippedWeapon->MaxWeaponAmmo - EquippedWeapon->WeaponAmmo),0,GetCarriedAmmo());
	EquippedWeapon->WeaponAmmo += GetCarriedAmmo() - temp;
	SetCarriedAmmo(temp);
	GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
	if (IsLocallyControlled())
	{
		GetBlasterWidget()->SetBlasterAmmo(GetCarriedAmmo());
		GetBlasterWidget()->SetWeaponAmmo(EquippedWeapon->WeaponAmmo);
	}
}
