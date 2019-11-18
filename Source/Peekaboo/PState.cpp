// Fill out your copyright notice in the Description page of Project Settings.

#include "Peekaboo.h"
#include "UnrealNetwork.h"
#include "PC.h"
#include "PState.h"
#include "GS.h"


APState::APState()
{
	health = 100;

}

void APState::CopyProperties(APlayerState * PlayerState)
{
	Super::CopyProperties(PlayerState);
	if (PlayerState) {
		APState* TestPlayerState = Cast<APState>(PlayerState);
		if (TestPlayerState)
		{
			TestPlayerState->health = 1000;
			PName = TestPlayerState->PName;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(TestPlayerState->health));

		}
	}

}

void APState::OverrideWith(APlayerState * PlayerState)
{
	Super::OverrideWith(PlayerState);
	if (PlayerState) {
		APState* TestPlayerState = Cast<APState>(PlayerState);
		if (TestPlayerState)
		{
			PName = TestPlayerState->PName;
			int32 SomeVariable = TestPlayerState->health;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "debug msg "  + FString::FromInt(SomeVariable));
		}


	}


}

void APState::SeamlessTravelTo(APlayerState * PlayerState)
{
	Super::SeamlessTravelTo(PlayerState);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SeamlessTravelTo msg");

}

void APState::SetHealth(int32 damage)
{
	health -= damage;
}

void APState::SetHasBall(bool SetHasBall)
{
	hasBall = SetHasBall;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SetHasBall " + FString::FromInt(SetHasBall));
	

}

void APState::SetName(FText input)
{


	if (Role == ROLE_Authority)
	{
		PName = input;
		return;
	}
	const FString Reference = input.ToString();

	// ServerSetName(Reference, this);
}


/*
void APState::ServerSetName_Implementation(FString input, APState* inputState) const
{
	APState* TempState = Cast<APState>(inputState);
	TempState->PName = FText::FromString(input);
	GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Red, "NameChange: ");
}

bool APState::ServerSetName_Validate(FString input, APState* inputState) const
{
	return true;
}
*/


void APState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(APState, health);
	DOREPLIFETIME(APState, hasBall);
	DOREPLIFETIME(APState, Team);
	DOREPLIFETIME(APState, Deaths);
	DOREPLIFETIME(APState, Kills);
	DOREPLIFETIME(APState, Position);
	DOREPLIFETIME(APState, PName);
}