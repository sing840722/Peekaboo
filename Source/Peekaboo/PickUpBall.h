// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PChar.h"
#include "PickUpBall.generated.h"

UCLASS()
class PEEKABOO_API APickUpBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpBall();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
//	virtual void Tick( float DeltaSeconds ) override;

	UStaticMeshComponent* SphereVisual;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
