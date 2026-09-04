// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Case.generated.h"

UCLASS()
class BLASTER_API ACase : public AActor
{
	GENERATED_BODY()

public:

	ACase();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "CPPVars")
	UStaticMeshComponent* CaseMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "CPPVars")
	USoundBase* ImpactSound;
	
};
