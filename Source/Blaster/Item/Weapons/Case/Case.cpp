// Fill out your copyright notice in the Description page of Project Settings.


#include "Case.h"

#include "Kismet/GameplayStatics.h"


ACase::ACase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CaseMesh"));
	RootComponent = CaseMesh;
	
	CaseMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CaseMesh->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	
	CaseMesh->SetSimulatePhysics(true);
	CaseMesh->SetEnableGravity(true);
	CaseMesh->SetNotifyRigidBodyCollision(true);
}

void ACase::BeginPlay()
{
	Super::BeginPlay();
	
	CaseMesh->OnComponentHit.AddDynamic(this, &ACase::OnHit);
	
	CaseMesh->AddImpulse(GetActorForwardVector() * 10);
}

void ACase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.ImpactPoint);
	Destroy();
}
