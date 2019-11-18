// Fill out your copyright notice in the Description page of Project Settings.

#include "Peekaboo.h"
#include "UIGM.h"
#include "Public/Blueprint/UserWidget.h"
#include "HttpModule.h"
#include "Http.h"
#include "IHttpRequest.h"
#include "Engine.h"


AUIGM::AUIGM()
{
	//Add all the rank names to the rank string array. This is used for uploading to the database and retrieving from the database
	RankArray.Add(TEXT("Rank0"));
	RankArray.Add(TEXT("Rank1"));
	RankArray.Add(TEXT("Rank2"));
	RankArray.Add(TEXT("Rank3"));
	RankArray.Add(TEXT("Rank4"));
}

FText AUIGM::UploadTable(int32 Option)
{
	if (ScoreArray.IsValidIndex(0) == true)
	{
		//Convert text to string		
		FString SendString;
		FString ScoreSerialised;

		//Creating a  Json object that we will use to package our smaller objects
		TSharedPtr<FJsonObject> MainObject = MakeShareable(new FJsonObject);
		//Creating a series of objects that will be packaged into the bigger object 
		for (int32 i = 0; i < 5; i++)
		{
			TSharedPtr<FJsonObject> RankObject = MakeShareable(new FJsonObject);
			RankObject->SetStringField("Username", AttackingStringArray[i]);
			RankObject->SetNumberField("Highest", ScoreArray[i]);
			FString Arraystring = FString::FromInt(i);
			MainObject->SetObjectField("Rank" + Arraystring, RankObject);
		}
		//Serialises the object 
		TSharedRef<TJsonWriter<>>ScoreJWriter = TJsonWriterFactory<>::Create(&ScoreSerialised);
		//Creates a HTTP Request
		TSharedRef<IHttpRequest> AHttp = FHttpModule::Get().CreateRequest();
		switch (Option)
		{
		default:
			break;

			//Use the attacking table
		case 1:  SendString = "https://amber-heat-4334.firebaseio.com/PeekabooTable/Attackers/.json";
			FJsonSerializer::Serialize(MainObject.ToSharedRef(), ScoreJWriter);
			AHttp->SetVerb("PUT");
			AHttp->SetURL(SendString);
			AHttp->SetHeader("Content-Type", "application/json");
			AHttp->SetContentAsString(ScoreSerialised);
			AHttp->ProcessRequest();
			break;
			//Use the defending table
		case 2: SendString = "https://amber-heat-4334.firebaseio.com/PeekabooTable/Defenders/.json";
			FJsonSerializer::Serialize(MainObject.ToSharedRef(), ScoreJWriter);
			AHttp->SetVerb("PUT");
			AHttp->SetURL(SendString);
			AHttp->SetHeader("Content-Type", "application/json");
			AHttp->SetContentAsString(ScoreSerialised);
			AHttp->ProcessRequest();
			break;
		}

		return FText::FromString(SendString);
	}
	else
		return FText::FromString(TEXT("There are no values to return"));

}


void AUIGM::RetrieveHighScores(int32 Option)
{
	FString urls;
	TSharedRef<IHttpRequest>Retrieve = FHttpModule::Get().CreateRequest();
	switch (Option)
	{
	case 1:  AND = "https://amber-heat-4334.firebaseio.com/PeekabooTable/Attackers/";
		urls = AND + Pretty;
		Retrieve->SetURL(urls);
		Retrieve->SetVerb("GET");
		Retrieve->SetHeader("Content-Type", "Application/json");
		Retrieve->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Retrieve->ProcessRequest();
		Retrieve->OnProcessRequestComplete().BindUObject(this, &AUIGM::HighscoreResponse);
		break;

	case 2:  AND = "https://amber-heat-4334.firebaseio.com/PeekabooTable/Defenders/";

		urls = AND + Pretty;
		Retrieve->SetURL(urls);
		Retrieve->SetVerb("GET");
		Retrieve->SetHeader("Content-Type", "Application/json");
		Retrieve->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Retrieve->ProcessRequest();
		Retrieve->OnProcessRequestComplete().BindUObject(this, &AUIGM::HighscoreResponse);
		break;
	default:
		break;

		return;
	}
}


void AUIGM::HighscoreResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful == true)
	{
		TSharedPtr<FJsonObject> Highscores;
		TSharedRef<TJsonReader<>> Read = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		AttackingStringArray.Empty();
		ScoreArray.Empty();

		if (FJsonSerializer::Deserialize(Read, Highscores))
		{
			for (FString Rank : RankArray)
			{
				AttackingStringArray.Add(Highscores->GetObjectField(Rank)->GetStringField(TEXT("Username")));
				ScoreArray.Add(Highscores->GetObjectField(Rank)->GetIntegerField(TEXT("Highest")));
			}

		}
	}

}


void AUIGM::SortTheTable(FString UName, int32 UScore)
{
	if (ScoreArray.IsValidIndex(0) == true)
	{

		if ((UScore > ScoreArray[4]) && (UScore < ScoreArray[3]))
		{
			ScoreArray[4] = UScore;
			AttackingStringArray[4] = UName;
		}
		else if ((UScore > ScoreArray[3]) && (UScore < ScoreArray[2]))
		{
			ScoreArray[4] = ScoreArray[3];
			AttackingStringArray[4] = AttackingStringArray[3];
			ScoreArray[3] = UScore;
			AttackingStringArray[3] = UName;

		}

		else if ((UScore > ScoreArray[2]) && (UScore < ScoreArray[1]))
		{
			ScoreArray[4] = ScoreArray[3];
			AttackingStringArray[4] = AttackingStringArray[3];
			ScoreArray[3] = ScoreArray[2];
			AttackingStringArray[3] = AttackingStringArray[2];
			ScoreArray[2] = UScore;
			AttackingStringArray[2] = UName;

		}
		else if ((UScore > ScoreArray[1]) && (UScore < ScoreArray[0]))
		{
			ScoreArray[4] = ScoreArray[3];
			AttackingStringArray[4] = AttackingStringArray[3];
			ScoreArray[3] = ScoreArray[2];
			AttackingStringArray[3] = AttackingStringArray[2];
			ScoreArray[2] = ScoreArray[1];
			AttackingStringArray[2] = AttackingStringArray[1];
			ScoreArray[1] = UScore;
			AttackingStringArray[1] = UName;
		}

		else if (UScore > ScoreArray[0])
		{
			ScoreArray[4] = ScoreArray[3];
			AttackingStringArray[4] = AttackingStringArray[3];
			ScoreArray[3] = ScoreArray[2];
			AttackingStringArray[3] = AttackingStringArray[2];
			ScoreArray[2] = ScoreArray[1];
			AttackingStringArray[2] = AttackingStringArray[1];
			ScoreArray[1] = ScoreArray[0];
			AttackingStringArray[1] = AttackingStringArray[0];
			ScoreArray[0] = UScore;
			AttackingStringArray[0] = UName;
		}
	}

	else
		return;
}