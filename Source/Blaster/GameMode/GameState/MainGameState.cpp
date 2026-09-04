// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"

#include "Blaster/Character/Controller/BlasterController.h"
#include "Blaster/Character/Controller/UserWidgets/InGameWidget.h"
#include "Blaster/Character/PlayerState/BlasterState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AMainGameState::AMainGameState()
{
	bReplicates = true;
}

void AMainGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMainGameState, Winners);
}

void AMainGameState::OnRep_BlasterStates()
{
	FString Stemp;
	for (auto Atemp : Cast<AMainGameState>(UGameplayStatics::GetGameState(this))->Winners)
	{
		Stemp += FString::Printf(TEXT("%s \n"),*Atemp );
	}

	Cast<ABlasterController>(UGameplayStatics::GetPlayerController(this, 0))->GetOwnedWidget()->SetInfo(FString("Winners : \n") + Stemp);
}

void AMainGameState::GetHighestKills()
{
	int8 Score = 1;
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		auto temp = Cast<ABlasterController>(*It)->GetPlayerState<ABlasterState>()->Kills;
		if (Score < temp)
		{
			Winners.Empty();
			Winners.AddUnique(Cast<ABlasterController>(*It)->GetName());
			Score = temp;
		}
		else if (Score == temp)
		{
			Winners.AddUnique(Cast<ABlasterController>(*It)->GetName());
		}
	}
}
