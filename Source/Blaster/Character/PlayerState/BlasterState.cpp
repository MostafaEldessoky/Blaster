// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterState.h"

#include "Blaster/Character/Controller/BlasterController.h"
#include "Blaster/Character/Controller/UserWidgets/InGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


UInGameWidget* ABlasterState::GetBlasterWidget()
{
	BlasterWidget = BlasterWidget == nullptr ? Cast<ABlasterController>(UGameplayStatics::GetPlayerController(this,0))->GetOwnedWidget() : BlasterWidget;
	return BlasterWidget;
}

void ABlasterState::BeginPlay()
{
	Super::BeginPlay();
	GetBlasterWidget();
}

bool ABlasterState::bIsLocallyControlled()
{
	return GetPawn()->GetController()->IsLocalController();
}

void ABlasterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABlasterState, Kills, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ABlasterState, Deaths, COND_OwnerOnly);
}

void ABlasterState::GotKill()
{
	Kills++;
	if (bIsLocallyControlled() && BlasterWidget) BlasterWidget->SetKills(Kills);
}

void ABlasterState::GotDeath()
{
	Deaths++;
	if (bIsLocallyControlled() && BlasterWidget) BlasterWidget->SetDeaths(Deaths);
}

void ABlasterState::OnRep_Kills()
{
	if (BlasterWidget) BlasterWidget->SetKills(Kills);
}

void ABlasterState::OnRep_Deaths()
{
	if (BlasterWidget) BlasterWidget->SetDeaths(Deaths);
}
