// Fill out your copyright notice in the Description page of Project Settings.


#include "Puff.h"

#include "Components/SphereComponent.h"


APuff::APuff()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PuffMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PuffMesh"));
	SetRootComponent(PuffMesh);
	SpereComponent->SetupAttachment(GetRootComponent());
	PuffMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APuff::PickUp(ABlasterCharacter* BlasterPtr)
{
	Destroy();
}


void APuff::BeginPlay()
{
	Super::BeginPlay();
	switch (PuffType)
	{
		case EPuffType::Heal:
		Tags.Add("PuffHeal");
			break;
		case EPuffType::Jump:
		Tags.Add("PuffJump");
			break;
		case EPuffType::Speed:
		Tags.Add("PuffSpeed");
			break;
		case EPuffType::Shield:
		Tags.Add("PuffShield");
			break;
		case EPuffType::Ammo:
		Tags.Add("PuffAmmo");
			break;
	}
	
}


