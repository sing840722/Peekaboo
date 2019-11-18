// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "PC.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonSerializer.h"
#include "JsonWriter.h"
#include "JsonUtilities.h"
#include "UIGM.generated.h"



/**
 *
 */
UCLASS()
class PEEKABOO_API AUIGM : public AGameMode
{
	GENERATED_BODY()
public:

	AUIGM();

	/*------------------------------------------------------------------Highscore  Methods--------------------------------------------*/

	//Exposed function for pushing values to the Attacking highscore
	UFUNCTION(BlueprintCallable, Category = "Push Attacking HighScore")
		FText UploadTable(int32 Option);

	//Exposed functtion for calling the highscore data
	UFUNCTION(BlueprintCallable, Category = "Retrieve Defending Highscores")
		void RetrieveHighScores(int32 Option);

	//Function responsible for handling the response 
	void HighscoreResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Push to games state")
		void SortTheTable(FString UName, int32 UScore);

	/*--------------------------------------------------------HighScore Storage----------------------------------------------------------*/

	//Array for holding the names we display
	UPROPERTY(BlueprintReadWrite)
		TArray<FString>AttackingStringArray;

	//Array for holding the scores we display
	UPROPERTY(BlueprintReadWrite)
		TArray<int32>ScoreArray;

	//Array for storang the rank strings for the http request
	UPROPERTY(BlueprintReadWrite)
		TArray<FString> RankArray;

	//String for deciding which url to use for http request
	FString AND;

	//Self Explanatory
	FString Pretty = (TEXT(".json?print=pretty"));
};


