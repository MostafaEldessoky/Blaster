// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Interfaces/PickUp.h"
#include "Components/SphereComponent.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	SpereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SpereComponent"));
	SetRootComponent(SpereComponent);
	
	SpereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SpereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SpereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	bNetLoadOnClient = true;
	bReplicates = true;
	SetReplicatingMovement(true);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	SpereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	SpereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
	if (HasAuthority())
	{
		SpereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPickUp>())
	{
		Cast<IPickUp>(OtherActor)->Overlapping(this,true);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UPickUp>())
	{
		Cast<IPickUp>(OtherActor)->Overlapping(this,false);
	}
}

void AItem::PickUp(ABlasterCharacter* BlasterPtr)
{
	SetOwner(BlasterPtr);
	SpereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItem::OnRep_Owner()
{
	Super::OnRep_Owner();
	SpereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AItem::Drop()
{ 
	SetOwner(nullptr);
	SpereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


