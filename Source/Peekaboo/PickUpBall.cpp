// Fill out your copyright notice in the Description page of Project Settings.

#include "Peekaboo.h"
#include "PChar.h"
#include "PState.h"
#include "PickUpBall.h"
#include "GS.h"
#include "PC.h"
#include "UnrealNetwork.h"


// Sets default values
APickUpBall::APickUpBall()
{
	PrimaryActorTick.bCanEverTick = false;


	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(120.0f);
	//SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/FinishedArt/CollectPoint_a/Ball_LP.Ball_LP"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0, 0, -100));
	}

	SphereVisual->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SphereVisual->SetCollisionResponseToAllChannels(ECR_Block);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickUpBall::OnOverlapBegin);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetSimulatePhysics(true);

	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);

}

// Called when the game starts or when spawned
void APickUpBall::BeginPlay()
{
	Super::BeginPlay();

	
}




void APickUpBall::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this)return;
	
	APChar* tempChar = Cast<APChar>(OtherActor);
	if (!tempChar)return;
	APState* ps = Cast<APState>(tempChar->PlayerState);
	if (!ps)return;
	for (TObjectIterator<APlayerController> Itr; Itr; ++Itr)
	{
		APC* tempPc = Cast<APC>(*Itr);
		APState* tempPs = Cast<APState>(tempPc->PlayerState);
		tempPs->hasBall = false;
	}	
	ps->hasBall = true;
	AGS* gs = Cast<AGS>(GetWorld()->GetAuthGameMode()->GameState);
	gs->WhoHasTheBall = ps->GetTeam();

	USphereComponent* SphereComponent = Cast<USphereComponent>(GetRootComponent());
	SphereComponent->SetSimulatePhysics(false);

	
	this->AttachToComponent(tempChar->GetBallActor(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	
	//this->TeleportTo(tempChar->GetBallActor()->GetComponentLocation(), FRotator(0, 0, 0), false, false);
}


