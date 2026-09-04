// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnim.h"

#include "../BlasterCharacter.h"
#include "Blaster/Item/Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBlasterAnim::GetOwnedCharacter()
{
	BlasterPtr = BlasterPtr == nullptr ? Cast<ABlasterCharacter>(TryGetPawnOwner()) : BlasterPtr;
}

void UBlasterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	GetOwnedCharacter();
}

void UBlasterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!BlasterPtr) return;
	
	XYSpeed = FVector2D(BlasterPtr->GetCharacterMovement()->Velocity.X, BlasterPtr->GetCharacterMovement()->Velocity.Y).Size();
	
	BlasterPtr->UpdateAnimations(BlasterPtr->GetControlRotation());
	
	XSpeed = BlasterPtr->XSpeed;
	YSpeed = BlasterPtr->YSpeed;
	XYAngle = BlasterPtr->XYAngle;
	XZAngle = BlasterPtr->XZAngle;
	
	bIsFalling = BlasterPtr->GetCharacterMovement()->IsFalling();
	
	if (!BlasterPtr->EquippedWeapon)
	{
		bHasWeapon = false;
	}
	else
	{
		bHasWeapon = true;
	}
	
	bIsCrouching = BlasterPtr->GetCharacterMovement()->IsCrouching();
	
	bISAiming = BlasterPtr->bAiming;
	
	bISDead = BlasterPtr->Health <= 0.f;	
	
}
