// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BlasterState.generated.h"

class UInGameWidget;
/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterState : public APlayerState
{
	GENERATED_BODY()

public:
	
	void GotKill();
	
	void GotDeath();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing= OnRep_Kills)
	int8 Kills{0};
	
	UPROPERTY(ReplicatedUsing= OnRep_Deaths)
	int8 Deaths{0};

protected:
	
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY()
	UInGameWidget* BlasterWidget;
	
	UInGameWidget* GetBlasterWidget();
	
	bool bIsLocallyControlled();
	
	UFUNCTION()
	void OnRep_Kills();
	
	UFUNCTION()
	void OnRep_Deaths();
	
};
