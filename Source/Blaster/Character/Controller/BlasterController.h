// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Constants/Enums.h"
#include "GameFramework/PlayerController.h"
#include "BlasterController.generated.h"


class UInGameWidget;
class UCrossFire;
class AItem;
struct FInputActionValue;
class UInputAction;
class ABlasterCharacter;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void AcknowledgePossession(APawn* InPawn) override;
	
	UInGameWidget* GetOwnedWidget();
	
	EStateTimer StateTimer{EStateTimer::Cooldown};
	
	float TimerForState{0.f};
	
	UPROPERTY()
	ABlasterCharacter* BlasterPtr;
	
	UPROPERTY()
	UInGameWidget* InGameWidget;
	
	UFUNCTION(Client, Reliable)
	void ClintTimers(EStateTimer State,float WarmUpTimerv,float MatchTimerv,float CooldownTimerv);

protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
    
    UPROPERTY(EditDefaultsOnly,Category="CPPVars|MappingContexts")
    TArray<UInputMappingContext*> MappingContexts;
    
    UPROPERTY(EditDefaultsOnly,Category="CPPVars|InputActions")
    UInputAction* MoveAction;
    
    UPROPERTY(EditDefaultsOnly,Category="CPPVars|InputActions")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly,Category="CPPVars|InputActions")
    UInputAction* JumpAction;
    
    UPROPERTY(EditDefaultsOnly,Category="CPPVars|InputActions")
    UInputAction* CrouchAction;
    
    UPROPERTY(EditDefaultsOnly,Category="CPPVars|InputActions")
    UInputAction* PickUpAction;
    
    UPROPERTY(EditDefaultsOnly,Category="CPPVars|InputActions")
    UInputAction* AimAction;
    
    UPROPERTY(EditDefaultsOnly,Category="CPPVars|InputActions")
    UInputAction* FireAction;
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars|InputActions")
	UInputAction* ReloadAction;
    
    UPROPERTY(EditDefaultsOnly,Category="CPPVars|Widgets")
    TSubclassOf<UInGameWidget> InGameWidgetClass;

private:
	
	FHitResult HitResult;
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	void Jump(const FInputActionValue& Value);
	
	void PickUp();
	
	void Crouch();
	
	void Aim(const FInputActionValue& Value);
	
	void Fire(const FInputActionValue& Value);
	
	void Reload(const FInputActionValue& Value);
	
	void LineTrace();
	
	UFUNCTION(Server, Reliable)
	void ServerSync(double ClintClock);
	
	UFUNCTION(Client, Reliable)
	void ClintSync(double ServerClock,double ClintClock);
	
	void SyncClock(float DeltaSeconds);
	
	void UpdateTimer(float DeltaSeconds);
	
	float TimeToSync{0.f};
	
	float TimeToUpdateTimer{1.f};
	
	double SyncTime;
	
};
