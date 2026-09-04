// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Constants/Enums.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

class ABlasterCharacter;
class ABlasterController;

namespace MatchState
{
	const FName WarmUpTimer = FName(TEXT("WarmUpTimer"));
	const FName CooldownTimer = FName(TEXT("CooldownTimer"));
}

/**
 * 
 */
UCLASS()
class BLASTER_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	AMainGameMode();
	
	virtual void Tick(float DeltaSeconds) override; 
	
	void Elim(ABlasterCharacter* Character);
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	float WarmUpTimer{5};
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	float MatchTimer{5};
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	float CooldownTimer{5};
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	float TimerToCheckState{3.f};
	
private:
	
	void Respawn();
	
	FTimerHandle ElimTimerHandle;
                   	
	UPROPERTY()
	ABlasterController* ElimBlasterController;
	
	UPROPERTY()
	ABlasterCharacter* ElimBlasterCharacter;
	
	void NotifyControllersState(EStateTimer State);
	
	void CustomStateTransition(float DeltaSeconds);
	
	
};
