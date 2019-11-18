// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "PState.generated.h"

/**
 * 
 */
UCLASS()
class PEEKABOO_API APState : public APlayerState
{
	GENERATED_BODY()

public:

		APState();

	//copy properties from current PS to passsd one 
	virtual void CopyProperties(class APlayerState* PlayerState) override;
	
	//ovverride current PS with the properities frop "CopyProperties" 
	virtual void OverrideWith(class APlayerState* PlayerState) override;

	virtual void SeamlessTravelTo(class APlayerState* PlayerState) override;

	UPROPERTY(Replicated)
		int32 health = 0;
	UPROPERTY(Replicated)
		int32 Deaths = 0;
	UPROPERTY(Replicated)
		int32 Kills = 0;
	UPROPERTY(Replicated)
		int32 Position = 0;
	UPROPERTY(Replicated, BlueprintReadWrite)
		FText PName;

	UPROPERTY(Replicated)
		bool hasBall = false;
	
	FORCEINLINE int32 GetHasBall()const { return hasBall; };

	UFUNCTION(BlueprintCallable, Category = "GetHealth")
	FORCEINLINE int32 GetHealth()const { return health; };

	UFUNCTION(BlueprintCallable, Category = "GetKills")
		FORCEINLINE int32 GetKills()const { return Kills; };

	UFUNCTION(BlueprintCallable, Category = "GetDeaths")
		FORCEINLINE int32 GetDeaths()const { return Deaths; };

	UFUNCTION(BlueprintCallable, Category = "GetPosition")
		FORCEINLINE int32 GetPosition()const { return Position; };

	UFUNCTION(BlueprintCallable, Category = "GetName")
		FORCEINLINE FText GetName()const { return PName; };

	void SetHealth(int32 damage);

	void SetHasBall(bool SetHasBall);

	UPROPERTY(Replicated, EditAnywhere, Category = "PSTeam")
		FText Team;


	FORCEINLINE void ResetHealth() { health = 100; };
	UFUNCTION(BlueprintCallable, Category = "GetTeam")
	FORCEINLINE FText GetTeam() const { return Team; };
	FORCEINLINE void SetTeam(FText input) { Team = input; };

	FORCEINLINE void SetPosition(int32 input) { Position = input; };
	
	UFUNCTION(BlueprintCallable, Category = "SetName")
		void SetName(FText input);

	//UFUNCTION(Server, WithValidation, Reliable)
	//void ServerSetName(FString input, APState* inputState) const;
	
};
