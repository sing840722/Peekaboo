// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PC.generated.h"

/**
 * 
 */
UCLASS()
class PEEKABOO_API APC : public APlayerController
{
	GENERATED_BODY()
	
public:
	APC();

	int32 lol;

	virtual void BeginPlay() override;

	UFUNCTION(Server, WithValidation, Reliable)
		void ServerSpawnSpectate();

	void ServerRespawnTimer();
	void ServerRespawn();
		
	UPROPERTY(EditAnywhere, Category = "Respawn")
		int32 RespawnCount;
	//Stores the respawn timer handle
	FTimerHandle RespawnTimerHandle;
	
	UFUNCTION(Client, Reliable)
		void ShowGameHud();


	UFUNCTION(Client, Reliable)
	void middleman(int32 Option);

	UFUNCTION(Client, Reliable)
		void ShowGameHudElements(int32 Option, UUserWidget* Reference);

	FTransform SpawnTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> wHolderHUD;
		
		UUserWidget* wGameHud;
		//Used to bind various data to an object
		FTimerDelegate TestObj;
		//Time handle has to be different to each other to stop the overide
		FTimerHandle HitmarkHandle;
		//Time handle has to be different to each other to stop the overide
		FTimerHandle BeenHitHandle;

		FTimerHandle DeadTextHandle;


		/************************************************************************/
		/* Round Materials                                                      */ 
		/************************************************************************/
	

		FTimerHandle RoundStartHandle;
		//---------------------------------------------Audio---------//
		UPROPERTY(EditDefaultsOnly, Category = "Sound")
			USoundCue* Music;
		void PlaySound(int32 Option);

		/************************************************************************/
		/* Opening the Tab                                                      */
		/************************************************************************/
		UFUNCTION()
			void OpenTab();
		UFUNCTION()
			void CloseTab();
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
			TSubclassOf<UUserWidget> WTabHolder;

		UUserWidget* wTab;


		/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			float BaseTurnRate;

		/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
			float BaseLookUpRate;

		UFUNCTION(BlueprintImplementableEvent)
			void Buzz();

};
