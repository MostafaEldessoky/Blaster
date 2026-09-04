// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;
/**
 * 
 */
UCLASS()
class BLASTER_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void InGameVisibility(bool bShow);
	
	void ToggelScope(bool bShow);
	
	void SetTimer(FString Text);
	
	void SetState(FString Text);

	void SetInfo(FString Text);

	void SetHealthBar(float Health, float MaxHealth);
	
	void SetShieldBar(float Shield, float MaxShield);
	
	void ModifyCrossHairPadding(float f);
    	
	void ModifyCrossHairColor(FLinearColor Color);
	
	void CrossHairVisibility(bool bShow);
	
	void SetPickUpMassage(FString Massage);
	
	void PickUpMassageVisibility(bool bShow);
	
	void SetWeaponAmmo(int8 Value);
	
	void SetBlasterAmmo(int8 Value);
	
	void SetKills(int8 Value);
	
	void SetDeaths(int8 Value);
	
	void SetJumpPuff(int8 Value);
	
	void SetSpeedPuff(int8 Value);
	
	void SetHealPuff(int8 Value);
	
	void SetShieldPuff(int8 Value);
	
protected:
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Timer;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* State;
	
	UPROPERTY(Meta = (BindWidget))
	UTextBlock* Info;
	
	UPROPERTY(Meta = (BindWidget))	
    UImage* CrossHairCenter;
    
    UPROPERTY(Meta = (BindWidget))
    UImage* CrossHairTop;
    
    UPROPERTY(Meta = (BindWidget))
    UImage* CrossHairBottom;
    
    UPROPERTY(Meta = (BindWidget))
    UImage* CrossHairRight;
    
    UPROPERTY(Meta = (BindWidget))
    UImage* CrossHairLeft;
	
	UPROPERTY(Meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(Meta = (BindWidget))
	UProgressBar* ShieldBar;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* PickUpMassage;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* Kills;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* Deaths;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* WeaponAmmo;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* BlasterAmmo;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* JumpPuff;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* SpeedPuff;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* HealPuff;
	
	UPROPERTY(Meta=(BindWidget))
	UTextBlock* ShieldPuff;
	
	UPROPERTY(Meta=(BindWidget))
	UImage* ScopeOuter;
	
	UPROPERTY(Meta=(BindWidget))
	UImage* ScopeInner;
	
};
