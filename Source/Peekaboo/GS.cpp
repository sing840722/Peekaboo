// Fill out your copyright notice in the Description page of Project Settings.

#include "Peekaboo.h"
#include "GS.h"
#include "PState.h"
#include "UnrealNetwork.h"



AGS::AGS()
{
	WhoHasTheBall = FText::FromString("noneOne");
	
}
/*
TArray<UObject> AGS::ReturnArray()
{
	TArray<UObject>ToReturn;
	for (APlayerState* LoopState : PlayerArray)
	{
		UObject* Obj;
		
		APState* CastState = Cast<APState>(LoopState);
		CastState->GetKills();
	}

	return ToReturn;
}
*/
void AGS::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGS, RedScore);
	DOREPLIFETIME(AGS, BlueScore);
	DOREPLIFETIME(AGS, Clock);
	DOREPLIFETIME(AGS, RedRoundScore);
	DOREPLIFETIME(AGS, BlueRoundScore);
	DOREPLIFETIME(AGS, RoundInt);
	
	

}