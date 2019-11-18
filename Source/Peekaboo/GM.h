// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UIGM.h"
#include "PC.h"
#include "PState.h"
#include "PChar.h"
#include "UDPSender.h"
#include "PickUpBall.h"

#include "GM.generated.h"

/**
 * 
 */
UCLASS()
class PEEKABOO_API AGM : public AGameMode
{
	GENERATED_BODY()
	
public:
	AGM();
	//----------------------------------------------------------Called before BeginPlay


	virtual void BeginPlay() override;
	/*--------------------------------------------------------RespawnStorage----------------------------------------------------------*/
	//Respawn Storage to pass through the timer
	APlayerController* TemporaryController;
	//Stores the respawn timer handle
	FTimerHandle RespawnTimerHandle;
	/*--------------------------------------------------------PostLogin----------------------------------------------------------*/

	//Linker post login override function
	virtual void PostLogin(APlayerController* NewPlayer) override;
	//Store the amount of players
	int32 PlayerCount;
	//Store array of player spawnLocations
	TArray<APlayerStart*> PlayerStartArray;
	TArray<APlayerStart*> PlayerStartArraySorted;

	//Storee array of player controllers
	TArray<APC*> PControllerArray;
	//Fill SpawnLocArray
	void InitSpawnLocArray();
	//FText
	FText TxtRed = FText::FromString("Red");
	FText TxtBlue = FText::FromString("Blue");
	//Spawn the player
	void SpawnPlayer(APC* PlayerController, APState* PlayerState);

	//the player to spawn
	UPROPERTY(EditAnywhere, Category = "ThePlayer")
		TSubclassOf<APChar>ThePlayer;

	//UFUNCTION(BlueprintImplementableEvent)
		void RemovePlayers(APlayerController* Target);

		
	/*--------------------------------------------------------Timer----------------------------------------------------------*/
	//Checks if the clock is going to tick down
	UFUNCTION(BlueprintCallable, Category = "GameClock")
		void GameClock(bool start);
	//Clock Timer 
	void ClockTickDown();
	//Int to store the clock number
	UPROPERTY(EditAnywhere, Category = "ClockVariable")
		int32 clock;
	//Hold the clock timer handle
	FTimerHandle GameEndingHandle;

	int32 RedScore;
	int32 BlueScore;

	void PreReset(FText winner);

	//--------------------------------------------------------Timer for PreReset-------------------------------------
	FTimerHandle PreResetHandle;
	void PreResetTickDown();
	void ResetGame();


	UPROPERTY()
	int32 PreResetClock = 5;

	//------------------------------------------------------------Array Of Playerstarts-------------

	int32 p = 0;

	//-------------------------------------------------------RoundInfor----//
	void DetermineWhatRoundWeAreOn();
	void GameEndingScreen(FText Winner);
	

	//----------------------------------------------------Replicate to GS---------///
	void ReplicateToGS(int32 BlueScoreinput, int32 RedScoreinput, int32 Clockinput);


	//-------------------------------------------------------Server stuff----//
	AUDPSender* client;

	//-------------------------------------------------------Ball---------//
	UPROPERTY(EditAnywhere, Category = "ThePlayer")
	TSubclassOf<APickUpBall>TheBall;
	APickUpBall* Ball;

	UPROPERTY(EditAnywhere, Category = "Amount Of Players")
		int32 MaxPlayers;

	};
