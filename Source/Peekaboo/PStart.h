// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PStart.generated.h"

/**
 * 
 */
UCLASS()
class PEEKABOO_API APStart : public AActor	// : public APlayerStart
{
	GENERATED_BODY()

	APStart() {}

	UPROPERTY(EditAnywhere, Category = "Tag")
		FText TeamTag;
};
