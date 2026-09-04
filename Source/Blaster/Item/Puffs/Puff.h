// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Constants/Enums.h"
#include "Blaster/Item/Item.h"
#include "Puff.generated.h"

UCLASS()
class BLASTER_API APuff : public AItem
{
	GENERATED_BODY()

public:

	APuff();
	
	virtual void PickUp(ABlasterCharacter* BlasterPtr) override;
	
	UPROPERTY(EditDefaultsOnly, Category="CPPVars")
	int Amount{5};
	
	UPROPERTY(EditDefaultsOnly, Category="CPPVars")
	float Duration{5.0f};
	
	UPROPERTY(EditDefaultsOnly,Category="CPPVars")
	EPuffType PuffType;
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,Category="CPPVars")
	UStaticMeshComponent* PuffMesh;
	
};
