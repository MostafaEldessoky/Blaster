// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UInGameWidget::ModifyCrossHairPadding(float f)
{
	CrossHairTop->SetRenderTranslation(FVector2D(0,-f));
	CrossHairBottom->SetRenderTranslation(FVector2D(0,f));
	CrossHairRight->SetRenderTranslation(FVector2D(f,0));
	CrossHairLeft->SetRenderTranslation(FVector2D(-f,0));
}

void UInGameWidget::ModifyCrossHairColor(FLinearColor Color)
{
	CrossHairCenter->SetColorAndOpacity(Color);
	CrossHairTop->SetColorAndOpacity(Color);
	CrossHairBottom->SetColorAndOpacity(Color);
	CrossHairRight->SetColorAndOpacity(Color);
	CrossHairLeft->SetColorAndOpacity(Color);
}

void UInGameWidget::CrossHairVisibility(bool bShow)
{
	CrossHairCenter->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairTop->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairBottom->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairRight->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairLeft->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInGameWidget::SetPickUpMassage(FString Message)
{
	PickUpMassage->SetText(FText::FromString(Message));
}

void UInGameWidget::PickUpMassageVisibility(bool bShow)
{
	PickUpMassage->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInGameWidget::SetWeaponAmmo(int8 Value)
{
	WeaponAmmo->SetText(FText::AsNumber(Value));
}

void UInGameWidget::SetBlasterAmmo(int8 Value)
{
	BlasterAmmo->SetText(FText::AsNumber(Value));
}

void UInGameWidget::SetKills(int8 Value)
{
	Kills->SetText(FText::AsNumber(Value));
}

void UInGameWidget::SetDeaths(int8 Value)
{
	Deaths->SetText(FText::AsNumber(Value));
}

void UInGameWidget::SetJumpPuff(int8 Value)
{
	JumpPuff->SetText(FText::AsNumber(Value));
}

void UInGameWidget::SetSpeedPuff(int8 Value)
{
	SpeedPuff->SetText(FText::AsNumber(Value));
}

void UInGameWidget::SetHealPuff(int8 Value)
{
	HealPuff->SetText(FText::AsNumber(Value));
}

void UInGameWidget::SetShieldPuff(int8 Value)
{
	ShieldPuff->SetText(FText::AsNumber(Value));
}

void UInGameWidget::InGameVisibility(bool bShow)
{
	PickUpMassage->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairCenter->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairTop->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairBottom->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairLeft->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	CrossHairRight->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	HealPuff->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	SpeedPuff->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	ShieldPuff->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	JumpPuff->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	BlasterAmmo->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	WeaponAmmo->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Kills->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	Deaths->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	HealthBar->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	ShieldBar->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	State->SetVisibility(bShow ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	Info->SetVisibility(bShow ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	ScopeOuter->SetVisibility(ESlateVisibility::Hidden);
	ScopeInner->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameWidget::ToggelScope(bool bShow)
{
	ScopeOuter->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	ScopeInner->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UInGameWidget::SetTimer(FString Text)
{
	Timer->SetText(FText::FromString(Text));
}

void UInGameWidget::SetState(FString Text)
{
	State->SetText(FText::FromString(Text));
}

void UInGameWidget::SetInfo(FString Text)
{
	Info->SetText(FText::FromString(Text));
}

void UInGameWidget::SetHealthBar(float Health, float MaxHealth)
{
	HealthBar->SetPercent(FMath::Clamp(Health / MaxHealth,0.f,1.f));
}

void UInGameWidget::SetShieldBar(float Shield, float MaxShield)
{
	ShieldBar->SetPercent(FMath::Clamp(Shield / MaxShield,0.f,1.f));
}

