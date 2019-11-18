// Fill out your copyright notice in the Description page of Project Settings.

#include "Peekaboo.h"
#include "Networking.h"
#include "PState.h"
#include "GS.h"
#include "UDPSender.h"
#include "PlayeDetails.h"



// Sets default values
AUDPSender::AUDPSender()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SenderSocket = NULL;
}

void AUDPSender::BeginPlay()
{
	names.Add(TEXT("Jake"));
	names.Add(TEXT("Moynul"));
	names.Add(TEXT("Moses"));
	names.Add(TEXT("Chris"));

}


// Called when the game starts or when spawned
void AUDPSender::Send(AGameState* gs)
{
	
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	int32 ThePort = 9050;
	FString TheIP(TEXT("127.0.0.1"));
	bool bIsValid;
	RemoteAddr->SetIp(*TheIP, bIsValid);
	RemoteAddr->SetPort(ThePort);
	FString SocketName(TEXT("Socket"));
	SenderSocket = FUdpSocketBuilder(SocketName).AsReusable().WithBroadcast();
	int32 SendSize = 2 * 1024 * 1024;
	SenderSocket->SetSendBufferSize(SendSize, SendSize);
	SenderSocket->SetReceiveBufferSize(SendSize, SendSize);

	AGS* gst = Cast<AGS>(gs);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(gs->PlayerArray.Num()));

	for (int i = 0; i < gst->PlayerArray.Num(); i++) 
	{

	


		int32 BytesSent = 0;

		FPlayerDetails NewData;
		NewData.name = names[i];
		NewData.kill = FMath::FRandRange(0, 100);
		NewData.death = FMath::RandRange(0, 100);
		NewData.score = FMath::RandRange(0, 100);

		FArrayWriter Writer;

		Writer << NewData; //Serializing our custom data, thank you UE4!
		SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *RemoteAddr);
	}
		
	


	
}



// Called every frame
void AUDPSender::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

