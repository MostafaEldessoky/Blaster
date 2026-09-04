// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class ABlasterCharacter;

UCLASS(Abstract)
class BLASTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	
	AItem();
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	FString PickUpMassage;
	
	virtual void PickUp(ABlasterCharacter* BlasterPtr);
	virtual void Drop();
	
	UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	
	virtual void OnRep_Owner() override;
	
	virtual void BeginPlay() override;
    
	UPROPERTY(VisibleAnywhere, Category = "CPPVars")
	USphereComponent* SpereComponent;
	
};
