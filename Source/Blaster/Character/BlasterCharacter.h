// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Constants/Enums.h"
#include "Blaster/Interfaces/PickUp.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

class UTimelineComponent;
class AMainGameMode;
class UInGameWidget;
class ABlasterController;
class AWeapon;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IPickUp
{
	GENERATED_BODY()

public:
	
	ABlasterCharacter();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float deltatime) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health = 100.f;
    	
    UFUNCTION()
    void OnRep_Health();
	
	UPROPERTY(ReplicatedUsing= OnRep_OverlappingItem)
	AItem* OverlappingItem = nullptr;
	
	UFUNCTION()
	void OnRep_OverlappingItem(AItem* OverlappedItem);
	
	virtual void Overlapping(AItem* Item,bool bIsOverlapping) override;
	
	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon = nullptr;
	
	UFUNCTION()
	void OnRep_EquippedWeapon();
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	FName WeaponSocketName;
	
	UFUNCTION(Server, Reliable)
	void ServerPickUp();
	
	UFUNCTION(Server, Reliable)
	void ServerAim(bool bISAiming);
	
	UFUNCTION(Server, Reliable)
	void ServerStrafing(FRotator NewRotation);
	
	void Strafing(FRotator NewRotation);
    	
	void UpdateAnimations(FRotator NewRotation);
	
	UFUNCTION(Server, Reliable)
	void ServerUpdateAnimations(FRotator NewRotation);
	
	UPROPERTY(Replicated)
	bool bAiming = false;
	
	UPROPERTY(Replicated)
	float XSpeed;
        	
	UPROPERTY(Replicated)
	float YSpeed;
    	
	UPROPERTY(Replicated)
	float XYAngle;
	
	UPROPERTY(Replicated)
	float XZAngle;
	
	void Zoom(bool bIsZooming);
	
	void PlayHitReactMontage(EHitReactMontage HitReactMontageName,EWeaponType ProjectileType);
	
	int8 GetCarriedAmmo();
	
	void SetCarriedAmmo(int8 Ammo);
	
	UPROPERTY(ReplicatedUsing=OnRep_CarriedAmmo)
	int8 CarriedAmmo{0};
	
	UFUNCTION()
	void OnRep_CarriedAmmo();
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	UAnimMontage* ReloadMontage;
	
	UFUNCTION(Server, Reliable)
	void ServerReload();

protected:
	
	UPROPERTY(VisibleAnywhere,Category ="CPPVars|Components")
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere,Category ="CPPVars|Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars|Components")
	TMap<EWeaponType,UAnimMontage*> HitReactMontage;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars|Components")
	FName HitReactMontageSectionName_FWD;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars|Components")
	FName HitReactMontageSectionName_BWD;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars|Components")
	FName HitReactMontageSectionName_LEFT;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars|Components")
	FName HitReactMontageSectionName_RIGHT;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars|Components")
	UAnimMontage* DeathMontage;

private:
	
	UPROPERTY()
	UInGameWidget* BlasterWidget;
	
	UInGameWidget* GetBlasterWidget();
	
	UPROPERTY()
	ABlasterController* BlasterController;
	
	ABlasterController* GetBlasterController();
	
	UPROPERTY()
	AMainGameMode* MainGameMode;
	
	AMainGameMode* GetMainGameMode();
	
	void OnDeath();
	
	bool bIsDead = false;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	UMaterialInstance* MaterialInstance;
	
	UMaterialInstanceDynamic* DynamicMaterialInstance;
	
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DesolveTimeline;
	FOnTimelineFloat OnTimelinefloat;
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	TWeakObjectPtr<UCurveFloat> DesolveCurve;
	UFUNCTION()
	void DesolveTimelineFloat(float value);
	void startDesolveTimeline();
	
	
	
	
};
