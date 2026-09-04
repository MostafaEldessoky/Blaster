// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterController.h"

#include "../BlasterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blaster/Character/PlayerState/BlasterState.h"
#include "Blaster/GameMode/MainGameMode.h"
#include "Blaster/GameMode/GameState/MainGameState.h"
#include "Blaster/Item/Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UserWidgets/InGameWidget.h"

UInGameWidget* ABlasterController::GetOwnedWidget()
{
	InGameWidget = InGameWidget == nullptr ? CreateWidget<UInGameWidget>(this, InGameWidgetClass) : InGameWidget;
	return InGameWidget;
}

void ABlasterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateTimer(DeltaSeconds);
	SyncClock(DeltaSeconds);
}

void ABlasterController::AcknowledgePossession (APawn* InPawn)
{
	Super::AcknowledgePossession (InPawn);
	
	BlasterPtr = Cast<ABlasterCharacter>(InPawn);
	
	if (!InGameWidget) return;
	InGameWidget->CrossHairVisibility(false);
	InGameWidget->PickUpMassageVisibility(false);
	InGameWidget->SetHealthBar(100.f,100.f);
	InGameWidget->SetShieldBar(100.f,100.f);
	InGameWidget->SetWeaponAmmo(0);
	InGameWidget->SetBlasterAmmo(BlasterPtr->GetCarriedAmmo());
	InGameWidget->SetKills(GetPlayerState<ABlasterState>()->Kills);
	InGameWidget->SetDeaths(GetPlayerState<ABlasterState>()->Deaths);
	InGameWidget->SetJumpPuff(0);
	InGameWidget->SetSpeedPuff(0);
	InGameWidget->SetHealPuff(0);
	InGameWidget->SetShieldPuff(0);
}

void ABlasterController::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;
	
	if (!IsLocalController()) return;
	GetOwnedWidget();
	if (!InGameWidget) return;
	 InGameWidget->AddToViewport();
}

void ABlasterController::SetupInputComponent()
{
	Super::SetupInputComponent();
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    	{
    		for (const auto& Context : MappingContexts)
    		{
    			Subsystem->AddMappingContext(Context, 0);
    		}
    	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &ThisClass::PickUp);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::Crouch);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::Aim);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ABlasterController::Fire);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABlasterController::Reload);
	}
}

void ABlasterController::Move(const FInputActionValue& Value)
{
	if (!BlasterPtr)return;
	if (BlasterPtr->EquippedWeapon)
	{
		
		BlasterPtr->Strafing(BlasterPtr->GetControlRotation());
		BlasterPtr->ServerStrafing(BlasterPtr->GetControlRotation());
	}
	BlasterPtr->AddMovementInput(FRotationMatrix(FRotator(0,GetControlRotation().Yaw,0)).GetUnitAxis(EAxis::X),Value.Get<FVector2D>().Y);
	BlasterPtr->AddMovementInput(FRotationMatrix(FRotator(0,GetControlRotation().Yaw,0)).GetUnitAxis(EAxis::Y),Value.Get<FVector2D>().X);
	if (!InGameWidget) return;
	Value.Get<FVector2D>().Size() > 0 ? InGameWidget->ModifyCrossHairPadding(10) : InGameWidget->ModifyCrossHairPadding(5);
}

void ABlasterController::Look(const FInputActionValue& Value)
{
	AddYawInput(Value.Get<FVector2D>().X);
	AddPitchInput(Value.Get<FVector2D>().Y);
	
	if (!BlasterPtr->EquippedWeapon || !InGameWidget) return;
	LineTrace();
	if (HitResult.GetActor() && HitResult.GetActor()->ActorHasTag("Player"))
	{ 
		InGameWidget->ModifyCrossHairColor(FLinearColor::Red);
	}
	else
	{
		InGameWidget->ModifyCrossHairColor(FLinearColor::White);
	}
}

void ABlasterController::Jump(const FInputActionValue& Value)
{
	if (!BlasterPtr)return;
	BlasterPtr->Jump();
	if (!InGameWidget) return;
	Value.Get<bool>() ? InGameWidget->ModifyCrossHairPadding(15) : InGameWidget->ModifyCrossHairPadding(5);
}

void ABlasterController::PickUp()
{
	if (!BlasterPtr)return;
	BlasterPtr->ServerPickUp();
	InGameWidget->ModifyCrossHairPadding(5);
	InGameWidget->CrossHairVisibility(true);
}

void ABlasterController::Crouch()
{
	if (BlasterPtr->GetCharacterMovement()->IsCrouching())
	{
		BlasterPtr->UnCrouch();
	}
	else
	{
		BlasterPtr->Crouch();
	}
}

void ABlasterController::Aim(const FInputActionValue& Value)
{
	BlasterPtr->ServerAim(Value.Get<bool>());
	if (!InGameWidget) return;
	Value.Get<bool>() ? InGameWidget->ModifyCrossHairPadding(-3) : InGameWidget->ModifyCrossHairPadding(5);
	InGameWidget->ToggelScope(Value.Get<bool>());
	BlasterPtr->Zoom(Value.Get<bool>());
}

void ABlasterController::Fire(const FInputActionValue& Value)
{
	if (!BlasterPtr)return;
	if (!BlasterPtr->EquippedWeapon)return;
	LineTrace();
	BlasterPtr->EquippedWeapon->ServerFire(HitResult.ImpactPoint);
	if (!InGameWidget) return;
	Value.Get<bool>() ? InGameWidget->ModifyCrossHairPadding(-2) : InGameWidget->ModifyCrossHairPadding(5);
	
}

void ABlasterController::Reload(const FInputActionValue& Value)
{
	if (!BlasterPtr)return;
	if (!BlasterPtr->EquippedWeapon)return;
	BlasterPtr->ServerReload();
}

void ABlasterController::LineTrace()
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector_NetQuantize WorldLocation;
	FVector_NetQuantize WorldDirection;
	DeprojectScreenPositionToWorld(ViewportSize.X / 2.f,ViewportSize.Y / 2.f, WorldLocation, WorldDirection);
	GetWorld()->LineTraceSingleByChannel(HitResult,
		WorldLocation + WorldDirection * 450,
		WorldLocation + WorldDirection * 15000 ,
		ECollisionChannel::ECC_Visibility);
	if (HitResult.ImpactPoint.IsZero())
	{
		HitResult.ImpactPoint = WorldLocation + WorldDirection * 15000;
	}
}

void ABlasterController::UpdateTimer(float DeltaSeconds)
{
	if (IsLocalController())
	{
		TimeToUpdateTimer -= DeltaSeconds;
		if(TimeToUpdateTimer <= 0.f)
		{
			FString timer;
			if (HasAuthority())
			{
				timer = FString::Printf(TEXT("%02d:%02d"), FMath::FloorToInt((-GetWorld()->GetTimeSeconds() + TimerForState) / 60), 
					FMath::FloorToInt( -GetWorld()->GetTimeSeconds() + TimerForState) - FMath::FloorToInt((-GetWorld()->GetTimeSeconds() + TimerForState) / 60)*60);
			}
			else
			{
				timer = FString::Printf(TEXT("%02d:%02d"), FMath::FloorToInt(( -GetWorld()->GetTimeSeconds() -SyncTime   + TimerForState) / 60), 
				FMath::FloorToInt(( -GetWorld()->GetTimeSeconds() -SyncTime + TimerForState) - FMath::FloorToInt(( -GetWorld()->GetTimeSeconds() -SyncTime  + TimerForState) / 60)*60));
			}
			GetOwnedWidget()->SetTimer(timer);
			TimeToUpdateTimer = 1.f;
		}
	}
}

void ABlasterController::ClintTimers_Implementation(EStateTimer State,float WarmUpTimerv,float MatchTimerv,float CooldownTimerv)
{
	switch (State)
	{
	case EStateTimer::WarmUp:
		InGameWidget->InGameVisibility(false);
		InGameWidget->SetState(FString("Game About To Start"));
		TimerForState = WarmUpTimerv + GetWorld()->GetTimeSeconds() + SyncTime;
		break;
	
	case EStateTimer::Match:
		InGameWidget->InGameVisibility(true);
		TimerForState = MatchTimerv + GetWorld()->GetTimeSeconds() + SyncTime;
		break;
		
	case EStateTimer::Cooldown:
		UnPossess();
		if (BlasterPtr) BlasterPtr->GetCharacterMovement()->DisableMovement();
		InGameWidget->InGameVisibility(false);
		if (HasAuthority())
		{
			FString Stemp = "";
			for (auto Atemp : Cast<AMainGameState>(UGameplayStatics::GetGameState(this))->Winners)
			{
				Stemp += FString::Printf(TEXT("%s \n"),*Atemp );
			}
			if (Stemp != "") Cast<ABlasterController>(UGameplayStatics::GetPlayerController(this, 0))->GetOwnedWidget()->SetInfo(FString("Winners : \n") + Stemp);
		}
		Cast<ABlasterController>(UGameplayStatics::GetPlayerController(this, 0))->GetOwnedWidget()->SetState(FString("Game About To Finished"));
		TimerForState = CooldownTimerv + GetWorld()->GetTimeSeconds() + SyncTime;
		break;
	}
}

void ABlasterController::SyncClock(float DeltaSeconds)
{
	if (HasAuthority()) return;
	TimeToSync -= DeltaSeconds;
	if(TimeToSync <= 0.f)
	{
		ServerSync(GetWorld()->GetTimeSeconds());
		TimeToSync = 30.f;
	}
}

void ABlasterController::ClintSync_Implementation(double ServerClock, double ClintClock)
{
	auto DeltaTrip = (GetWorld()->GetTimeSeconds() - ClintClock)/2;
	SyncTime =  ServerClock + DeltaTrip - GetWorld()->GetTimeSeconds();
}

void ABlasterController::ServerSync_Implementation(double ClintClock)
{
	ClintSync(GetWorld()->GetTimeSeconds(), ClintClock);
}
