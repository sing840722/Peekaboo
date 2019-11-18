// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "GS.generated.h"

/**
 * 
 */
UCLASS()
class PEEKABOO_API AGS : public AGameState
{
	GENERATED_BODY()
		AGS();

public:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "BallInfo")
		FText WhoHasTheBall;
			UPROPERTY(Replicated)
			int32 BlueScore;
			UPROPERTY(Replicated)
			int32 RedScore;

			UPROPERTY(Replicated)
				int32 Clock;

			UPROPERTY(Replicated)
			int32 RedRoundScore;
			UPROPERTY(Replicated)
			int32 BlueRoundScore;

			/*-----if true return red, else return blie*/
			UFUNCTION(BlueprintCallable, Category = "Team Score")
				FORCEINLINE int32 ReturnTeamScore(bool team) const { if (team)return RedScore; else return BlueScore; };

			UFUNCTION(BlueprintCallable, Category = "Team Score")
				FORCEINLINE int32 ReturnOverallScore(bool team) const { if (team)return RedRoundScore; else return BlueRoundScore; };

			UFUNCTION(BlueprintCallable, Category = "Clock Display")
				FORCEINLINE int32 ReturnTheClock() const { return Clock;};

			//UFUNCTION(BlueprintCallable, Category = "HighScore")
			//	TArray<UObject>ReturnArray();

			UPROPERTY(Replicated,BlueprintReadWrite, Category = "WidgetInt")
				int32 RoundInt;
		
	
	
};
