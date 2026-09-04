// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Constants/Enums.h"
#include "Blaster/Item/Item.h"
#include "Weapon.generated.h"

class ACase;

UCLASS(Abstract)
class BLASTER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, Category = "CPPVars")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	UAnimMontage* FireAnimation;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	FName HipFireMontageSection;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	FName AimFireMontageSection;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	int8 AimingFieldOfView = 60;
	
	void PlayFireMontage(FName MontageSection);
	
	UFUNCTION(Server, Reliable)
	void ServerFire(FVector_NetQuantize Point);
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MultiCastFire(FVector_NetQuantize Point);
	virtual void MultiCastFire_Implementation(FVector_NetQuantize Point);
	
	virtual void PickUp(ABlasterCharacter* BlasterPtr) override;
	
	virtual void Drop();
	
	void AutomaticFire();
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	int8 MaxWeaponAmmo{30};
	
	UPROPERTY(ReplicatedUsing=OnRep_WeaponAmmo,EditDefaultsOnly, Category = "CPPVars")
	int8 WeaponAmmo{MaxWeaponAmmo};
	
	UFUNCTION()
	void OnRep_WeaponAmmo();
	
	void UpdateAmmoWidget(int8 weaponAmmo);
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	EWeaponType WeaponType;
	
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	FName CaseSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	TSubclassOf<ACase> CaseClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	float FireRate = 1.f;

private:
	
	FTimerHandle FireTimer;
	
	bool bCurrentFiring = false;
};
