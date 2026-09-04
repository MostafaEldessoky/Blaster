// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Character/Controller/BlasterController.h"
#include "GameState/MainGameState.h"
#include "Kismet/GameplayStatics.h"


AMainGameMode::AMainGameMode()
{
	bDelayedStart = true;
	
}

void AMainGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CustomStateTransition(DeltaSeconds);
}

void AMainGameMode::Elim(ABlasterCharacter* Character)
{
	if (!Character) return;
	ElimBlasterCharacter = Character;
	ElimBlasterController = Cast<ABlasterController>(Character->GetController());
	if (ElimBlasterController) ElimBlasterController->UnPossess();
	GetWorldTimerManager().SetTimer(ElimTimerHandle,this,&AMainGameMode::Respawn,10.0f);
}

void AMainGameMode::Respawn()
{
	if (!ElimBlasterController) return;
	ElimBlasterCharacter->Destroy();
	RestartPlayerAtPlayerStart(ElimBlasterController,ChoosePlayerStart(ElimBlasterController));
}

void AMainGameMode::CustomStateTransition(float DeltaSeconds)
{
	TimerToCheckState -= DeltaSeconds;
	if (TimerToCheckState <=0)
	{
		if (MatchState == MatchState::WaitingToStart)
		{
			NotifyControllersState(EStateTimer::WarmUp);
			SetMatchState(MatchState::WarmUpTimer);
		}
		else if (MatchState == MatchState::WarmUpTimer)
		{
			WarmUpTimer -= 1;
			if (WarmUpTimer <= 0.f)
			{
				NotifyControllersState(EStateTimer::Match);
				SetMatchState(MatchState::EnteringMap);
				StartMatch();
			}
		}
		else if (MatchState == MatchState::InProgress)
		{
			MatchTimer -= 1;
			if (MatchTimer <= 0.f)
			{
				Cast<AMainGameState>(UGameplayStatics::GetGameState(this))->GetHighestKills();
				NotifyControllersState(EStateTimer::Cooldown);
				SetMatchState(MatchState::CooldownTimer);
			}
		}
		else if (MatchState == MatchState::CooldownTimer)
		{
			CooldownTimer -= 1;
			if (CooldownTimer <= 0.f)
			{
				SetMatchState(MatchState::InProgress);
				EndMatch();
			}
		}
		TimerToCheckState = 1.0f;
	}
}

void AMainGameMode::NotifyControllersState(EStateTimer State)
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		Cast<ABlasterController>(*It)->ClintTimers(State,WarmUpTimer + TimerToCheckState ,MatchTimer,CooldownTimer);
	}
}
