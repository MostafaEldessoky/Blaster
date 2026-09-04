// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MainGameState.generated.h"

class ABlasterController;
/**
 * 
 */
UCLASS()
class BLASTER_API AMainGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	
	AMainGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing=OnRep_BlasterStates)
	TArray<FString> Winners;
	
	UFUNCTION()
	void OnRep_BlasterStates();
	
	void GetHighestKills();
	
};
