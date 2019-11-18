// Fill out your copyright notice in the Description page of Project Settings.

#include "Peekaboo.h"
#include "PickUpBall.h"
#include "GS.h"
#include "GM.h"
#include "EngineUtils.h"



AGM::AGM()
{
	if (ThisClass::StaticClass() == AGM::StaticClass())
	{
		PlayerControllerClass = APC::StaticClass();
		DefaultPawnClass = NULL;
		PlayerStateClass = APState::StaticClass();
		GameStateClass = AGS::StaticClass();
	}
}


void AGM::BeginPlay()
{
	Super::BeginPlay();


	APC* tempFirstPlayer = Cast<APC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PControllerArray.Add(tempFirstPlayer);
	InitSpawnLocArray();
	for (int32 i(0); i < MaxPlayers; i++)
	{
		APC* pc = Cast<APC>(UGameplayStatics::CreatePlayer(GetWorld(), -1, true));
		PControllerArray.Add(pc);
		if (i == MaxPlayers - 1)
		{
			for (int32 j(0); j < PControllerArray.Num(); j++)
			{
				PControllerArray[j]->SpawnTransform = PlayerStartArray[j]->GetActorTransform();
				//APChar* NewPawn = GetWorld()->SpawnActor<APChar>(ThePlayer, PControllerArray[j]->SpawnTransform.GetLocation(), PControllerArray[j]->SpawnTransform.GetRotation().Rotator());
				//PControllerArray[j]->Possess(NewPawn);
				APState* tempPs = Cast<APState>(PControllerArray[j]->PlayerState);
				tempPs->SetPosition(j);
				int32 PStartNumber = FCString::Atoi(*PlayerStartArray[j]->PlayerStartTag.ToString());

				if (PStartNumber > 1)
				{
					tempPs->SetTeam(TxtRed);
				}
				else
					tempPs->SetTeam(TxtBlue);

				SpawnPlayer(PControllerArray[j], tempPs);

			}
		}




	}





	PreResetClock = 4;
	clock = 120;
	RedScore = 80;
	BlueScore = 80;
	ReplicateToGS(BlueScore, RedScore, clock);
	for (TActorIterator<AUDPSender> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		client = *ActorItr;
	}
	DetermineWhatRoundWeAreOn();
	for (APlayerController* Iter : PControllerArray)
	{
		APC* tempPC = Cast<APC>(Iter);
		tempPC->middleman(3);
		tempPC->PlaySound(0);

	}
	GameClock(true);


	//FVector spawnPos = FVector(-620.0f, 60.0f, 3560.0f);
//	FRotator spawnPos = FRotator(0.0f, 0.0f, 0.0f);

	//APickUpBall* Ball = GetWorld()->SpawnActor<APickUpBall>(TheBall, PlayerController->SpawnTransform.GetLocation(), PlayerController->SpawnTransform.GetRotation().Rotator());




}



void AGM::PostLogin(APlayerController * NewPlayer)
{
	if (PControllerArray.Num() > 3)
	{
		APC* specPC = Cast<APC>(NewPlayer);
		ASpectatorPawn* spec = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), PlayerStartArray[1]->GetActorTransform());
		specPC->Possess(spec);
	}
	return;
	//	ASpectatorPawn* Spectating;
		//	FActorSpawnParameters* Spawns = FActorSpawnParameters:
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	//Spectating = World->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), PControllerArray[2]->GetSpawnLocation());


}


void AGM::InitSpawnLocArray()
{
	for (TActorIterator<APlayerStart> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APlayerStart* PlayerStarts = *ActorItr;
		PlayerStartArray.Add(PlayerStarts);
	}
}

void AGM::SpawnPlayer(APC * PlayerController, APState* PlayerState)
{
	//check to see if we possessing anything
	ASpectatorPawn* spectatorPawn = Cast<ASpectatorPawn>(PlayerController->GetPawn());
	if (spectatorPawn)
	{
		spectatorPawn->Destroy();
		PlayerController->UnPossess();
	}
	APChar* NewPawn = GetWorld()->SpawnActor<APChar>(ThePlayer, PlayerController->SpawnTransform.GetLocation(), PlayerController->SpawnTransform.GetRotation().Rotator());
	if (NewPawn->Team.IsEmpty())
		NewPawn->Team = PlayerState->GetTeam();
	NewPawn->SetTeam();
	PlayerState->ResetHealth();
	PlayerController->Possess(NewPawn);
	PlayerController->ShowGameHud();
	NewPawn->PlaySound(4);
	//PlayerController->middleman(0);
}


void AGM::DetermineWhatRoundWeAreOn()
{
	AGS* GState = Cast<AGS>(GameState);
	int32 PassNumber = GState->RedRoundScore + GState->BlueRoundScore + 1;
	switch (PassNumber)
	{
	case 1: GState->RoundInt = 0;
		break;

	case 2: GState->RoundInt = 1;
		break;
	case 3: GState->RoundInt = 2;
		break;
	}
}

void AGM::ReplicateToGS(int32 BlueScoreinput, int32 RedScoreinput, int32 Clockinput)
{
	AGS* GState = Cast<AGS>(GameState);
	GState->BlueScore = BlueScoreinput;
	GState->RedScore = RedScoreinput;
	GState->Clock = Clockinput;

}




void AGM::RemovePlayers(APlayerController* Target)
{
	
	UGameInstance* GameStance = GetGameInstance();

		APlayerController* Controllerz = Target;
		APC* tempPawn = Cast<APC>(Target);
		if (tempPawn->wGameHud)
		{
			tempPawn->wGameHud->RemoveFromParent();
			if (tempPawn->wTab)
			{
				tempPawn->wTab->RemoveFromParent();
			}
		}

	
		if (Controllerz && Controllerz->IsLocalController() && !Controllerz->IsPrimaryPlayer())
		{
			ULocalPlayer* Removeplayer = Cast<ULocalPlayer>(Controllerz->Player);
			if (Removeplayer)
			{
			
				Controllerz->PawnPendingDestroy(Controllerz->GetPawn());
				GameStance->RemoveLocalPlayer(Removeplayer);

				
			}
		}
	}






void AGM::GameClock(bool start)
{
	UWorld* World = GetWorld();
	if (start == true)
	{
		GetWorldTimerManager().SetTimer(GameEndingHandle, this, &AGM::ClockTickDown, 1.0f, true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(GameEndingHandle);
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "Over");
		if (BlueScore > RedScore)PreReset(FText::FromString("Blue"));
		else if (RedScore > BlueScore)PreReset(FText::FromString("Red"));
		else PreReset(FText::FromString("Draw"));


	}
}

void AGM::ClockTickDown()
{

	AGS* GState = Cast<AGS>(GameState);
	if (clock >= 1)
	{

		//if (clock == 115)
		//	client->Send(GameState);
		clock -= 1;
		ReplicateToGS(BlueScore, RedScore, clock);

		if ((clock < 10) && (RedScore > 10) && (BlueScore > 10))
		{
			for (APlayerController* Iter : PControllerArray)
			{
				APChar* CastPawn = Cast<APChar>(Iter->GetPawn());
				if (!CastPawn)return;
				APC* BuzzPawn = Cast<APC>(Iter);
				BuzzPawn->Buzz();
				CastPawn->PlaySound(6);

			}
		}

		if (GState->WhoHasTheBall.EqualTo(FText::FromString("Red")))
		{
			BlueScore--;
			ReplicateToGS(BlueScore, RedScore, clock);
			if (BlueScore < 10)
			{
				for (APlayerController* Iter : PControllerArray)
				{
					APChar* CastPawn = Cast<APChar>(Iter->GetPawn());
					if (!CastPawn)return;
					CastPawn->PlaySound(6);
					APC* BuzzPawn = Cast<APC>(Iter);
					BuzzPawn->Buzz();
				}
			}
			if (BlueScore == 0)
			{
				PreReset(FText::FromString("Red"));
			}
		}


		else if (GState->WhoHasTheBall.EqualTo(FText::FromString("Blue")))
		{
			RedScore--;
			ReplicateToGS(BlueScore, RedScore, clock);
			if (RedScore < 10)
			{
				for (APlayerController* Iter : PControllerArray)
				{
					APChar* CastPawn = Cast<APChar>(Iter->GetPawn());
					if (!CastPawn)return;
					CastPawn->PlaySound(6);
					APC* BuzzPawn = Cast<APC>(Iter);
					BuzzPawn->Buzz();
				}
			}
			if (RedScore == 0)
			{
				PreReset(FText::FromString("Blue"));
			}
		}


	}
	else if (clock == 0)
		GameClock(false);
}

void AGM::PreReset(FText winner)
{
	GetWorldTimerManager().ClearTimer(GameEndingHandle);
	AGS* GState = Cast<AGS>(GameState);
	if (winner.EqualTo(FText::FromString("Red")))
	{
		GState->RedRoundScore++;
		GState->RoundInt = 4;
	}
	else if (winner.EqualTo(FText::FromString("Blue")))
	{
		GState->BlueRoundScore++;
		GState->RoundInt = 3;
	}
	else if (winner.EqualTo(FText::FromString("Draw")))
	{
		GState->RoundInt = 5;
	}
	GetWorldTimerManager().SetTimer(PreResetHandle, this, &AGM::PreResetTickDown, 1.0f, true);
	for (APlayerController* PC : PControllerArray)
	{
		APC* tempPC = Cast<APC>(PC);
		tempPC->middleman(3);
	}
}

void AGM::PreResetTickDown()
{
	AGS* GState = Cast<AGS>(GameState);
	if (PreResetClock <= 1)
	{
		if (GState->BlueRoundScore == 2)GameEndingScreen(FText::FromString("Blue"));	
		if( GState->RedRoundScore == 2)GameEndingScreen(FText::FromString("Red"));
		else
		ResetGame();
	}
	else
		PreResetClock--;
}

void AGM::ResetGame()
{
	GetWorldTimerManager().ClearTimer(PreResetHandle);
	GameClock(true);
	DetermineWhatRoundWeAreOn();
	for (TActorIterator<APickUpBall> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		APickUpBall* tempBall = Cast<APickUpBall>(*ActorItr);
		if (tempBall)
		{
			bool success = tempBall->TeleportTo(FVector(-620.0, 20.0, 3680.0), FRotator(0.0f, 0.0f, 0.0f));
		}
	}

	for (APlayerController* PC : PControllerArray)
	{
		APC* tempPC = Cast<APC>(PC);
		APChar* tempChar = Cast<APChar>(tempPC->GetPawn());
		if (!tempChar)return;
		tempChar->bGameEnd = true;
		tempChar->Death(tempChar);
		tempChar->bGameEnd = false;
		//tempPC->ServerSpawnSpectate();
		tempPC->middleman(3);
		AGS* GState = Cast<AGS>(GameState);
		GState->WhoHasTheBall = FText::FromString("Dick");
		PreResetClock = 4;
		clock = 120;
		RedScore = 80;
		BlueScore = 80;
		ReplicateToGS(BlueScore, RedScore, clock);
		//tempPC->PlaySound(0);

	}
}

void AGM::GameEndingScreen(FText Winner)
{

	AGS* GState = Cast<AGS>(GameState);

	if (GState->BlueRoundScore == 2)
	{
		for (APlayerController* PC : PControllerArray)
		{

			RemovePlayers(PC);

		}
		UGameplayStatics::OpenLevel((UObject*)GetGameInstance(), FName(TEXT("LobbyLevel")), true);

	}

	if (GState->RedRoundScore == 2)
	{
		for (APlayerController* PC : PControllerArray)
		{

			RemovePlayers(PC);

		}
		UGameplayStatics::OpenLevel((UObject*)GetGameInstance(), FName(TEXT("LobbyLevel")), true);
	}
}

