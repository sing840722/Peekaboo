// Fill out your copyright notice in the Description page of Project Settings.

#include "Peekaboo.h"
#include "PC.h"
#include "PState.h"
#include "PChar.h"
#include "GM.h"



APC::APC()
{
	RespawnCount = 4;
	BaseTurnRate = 60.f;
	BaseLookUpRate = 60.f;
	ForceFeedbackValues.LeftLarge = true;
	ForceFeedbackValues.LeftSmall = true;
	ForceFeedbackValues.RightLarge = true;
	ForceFeedbackValues.RightSmall = true;
}

bool APC::ServerSpawnSpectate_Validate() 
{
	return true;
}

void APC::ServerSpawnSpectate_Implementation()
{
	FTransform SpectatorLocation = GetPawn()->GetActorTransform();
	ASpectatorPawn* Spectating;
	//	FActorSpawnParameters* Spawns = FActorSpawnParameters:
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	Spectating = World->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), SpectatorLocation);
	
	bool TargetHasbeenDestroyed = GetPawn()->Destroy();
	if (TargetHasbeenDestroyed != true)
		return;
	UnPossess();
	Possess(Spectating);
	ServerRespawnTimer();
}

void APC::BeginPlay()
{
	
}

void APC::ServerRespawnTimer()
{	
	if (Role != ROLE_Authority) return;

	/*UWorld* World = GetWorld();
	if (World == nullptr)
		return;
		*/
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APC::ServerRespawn, 1.0f, true);
	
	

}

void APC::ServerRespawn()
{

	RespawnCount--;
	if (RespawnCount <= 1)
	{
		GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
		RespawnCount = 4;
		AGM* GMode = Cast<AGM>(GetWorld()->GetAuthGameMode());
		APState* playerState = Cast<APState>(this->PlayerState);
		GMode->SpawnPlayer(this, playerState);

	}
	
}

void APC::ShowGameHud_Implementation()
{

//	if (!IsLocalPlayerController())return;
	UWorld* World = GetWorld();
	if (!World)return;
	wGameHud = CreateWidget<UUserWidget>(World, wHolderHUD);
	if (!wGameHud)return;
	wGameHud->SetOwningLocalPlayer(GetLocalPlayer());
	wGameHud->AddToPlayerScreen();



	
	}
void APC::middleman_Implementation(int32 Option)
{
	if (!wGameHud)return;
	UImage* Hitmark = (UImage*)wGameHud->GetWidgetFromName(FName(TEXT("HitMark")));
	UImage* BloodSplat = (UImage*)wGameHud->GetWidgetFromName(FName(TEXT("BloodDamage")));
	UImage* RoundInformation = (UImage*)wGameHud->GetWidgetFromName(FName(TEXT("RoundInformation")));
	UImage* Deadmage = (UImage*)wGameHud->GetWidgetFromName(FName(TEXT("Deadmage")));
	switch (Option)
	{
	case 1:
		Hitmark->SetVisibility(ESlateVisibility::Visible);
		TestObj.BindUObject(this, &APC::ShowGameHudElements, 1, wGameHud);
		if (GetWorldTimerManager().IsTimerActive(HitmarkHandle))return;
		GetWorldTimerManager().SetTimer(HitmarkHandle, TestObj, 0.5, false);
		break;

	case 2:
	
		BloodSplat->SetVisibility(ESlateVisibility::Visible);
		TestObj.BindUObject(this, &APC::ShowGameHudElements, 2, wGameHud);
		if (GetWorldTimerManager().IsTimerActive(BeenHitHandle))return;
		GetWorldTimerManager().SetTimer(BeenHitHandle, TestObj, 0.5, false);
		break;

	case 3:
		RoundInformation->SetVisibility(ESlateVisibility::Visible);
		TestObj.BindUObject(this, &APC::ShowGameHudElements, 3, wGameHud);
		if (GetWorldTimerManager().IsTimerActive(RoundStartHandle))return;
		GetWorldTimerManager().SetTimer(RoundStartHandle, TestObj, 2.0, false);
		break;

	case 4:
		Deadmage->SetVisibility(ESlateVisibility::Visible);
		
		TestObj.BindUObject(this, &APC::ShowGameHudElements, 4, wGameHud);
		if (GetWorldTimerManager().IsTimerActive(DeadTextHandle))return;
		GetWorldTimerManager().SetTimer(DeadTextHandle, TestObj, 2.0, false);
		break;

	default:
		break;
	}
}
void APC::ShowGameHudElements_Implementation(int32 Option, UUserWidget* Reference)
{
	UImage* Hitmark = (UImage*)Reference->GetWidgetFromName(FName(TEXT("HitMark")));
	UImage* BloodSplat = (UImage*)Reference->GetWidgetFromName(FName(TEXT("BloodDamage")));
	UImage* RoundInformation = (UImage*)Reference->GetWidgetFromName(FName(TEXT("RoundInformation")));
	UImage* Deadmage = (UImage*)Reference->GetWidgetFromName(FName(TEXT("Deadmage")));
	switch (Option)
	{
	case 1:
		GetWorldTimerManager().ClearTimer(HitmarkHandle);
		Hitmark->SetVisibility(ESlateVisibility::Hidden);
		
		break;
	case 2: 
		GetWorldTimerManager().ClearTimer(BeenHitHandle);
		BloodSplat->SetVisibility(ESlateVisibility::Hidden);
		break;
	case 3:
		RoundInformation->SetVisibility(ESlateVisibility::Hidden);
		GetWorldTimerManager().ClearTimer(RoundStartHandle);
		break;
		
	case 4: 
		Deadmage->SetVisibility(ESlateVisibility::Hidden);
	//	Reference->SetColorAndOpacity(FColor::Blue);
		GetWorldTimerManager().ClearTimer(DeadTextHandle);
		break;
	default:
		break;
	}

}

void APC::PlaySound(int32 Option)
{
	switch (Option)
	{
	case 0 : //Play Music
		UGameplayStatics::SpawnSoundAttached(Music, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		break;


		break;

	}
}

void APC::OpenTab()
{
	UWorld* World = GetWorld();
	if (!World)return;
	wTab = CreateWidget<UUserWidget>(World, WTabHolder);
	if (!wTab)return;
	wTab->AddToViewport();
}

void APC::CloseTab()
{
	if (!wTab)return;
	wTab->RemoveFromViewport();
}


