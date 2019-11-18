// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UDPSender.generated.h"

UCLASS()
class PEEKABOO_API AUDPSender : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUDPSender();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	TSharedPtr<FInternetAddr>	RemoteAddr;

	FSocket* SenderSocket;

	void Send(AGameState* gs);

	TArray<FString> names;
	
};

