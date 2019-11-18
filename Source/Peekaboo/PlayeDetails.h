// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlayeDetails.generated.h"
/**
*
*/

USTRUCT(BlueprintType)
struct FPlayerDetails
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FPlayerDetails")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FPlayerDetails")
	int32 kill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FPlayerDetails")
	int32 score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FPlayerDetails")
	int32 death;
};
FORCEINLINE FArchive& operator<<(FArchive &Ar, FPlayerDetails& TheStruct)
{
	Ar << TheStruct.name;
	Ar << TheStruct.kill;
	Ar << TheStruct.score;
	Ar << TheStruct.death;
	return Ar;
}