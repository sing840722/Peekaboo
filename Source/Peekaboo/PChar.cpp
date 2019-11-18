// Fill out your copyright notice in the Description page of Project Settings.

#include "Peekaboo.h"
#include "PChar.h"
#include "PState.h"
#include "PC.h"
#include "PickUpBall.h"
#include "UnrealNetwork.h"
#include "GM.h"
#include "GS.h"

//include to send to server

// Sets default values
APChar::APChar()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	SpringArm->bUsePawnControlRotation = true;


	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);


	BallAddLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallPPosition"));
	BallAddLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	BallAddLocation->SetRelativeLocation(FVector(0.0f,0.0f,550.0f));


	UCapsuleComponent* myCap = GetCapsuleComponent();
	myCap->SetCapsuleRadius(100.0f);
	myCap->SetCapsuleHalfHeight(100.0f);
	//myCap->OnComponentBeginOverlap.AddDynamic(this, &APChar::OnOverlapBegin);




	LaserEmitParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserEmitParticle"));
	LaserEmitParticle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	LaserEmitParticle->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	RedParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RedParticle"));
	BlueParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BluePArticle"));
	Zoomzoomzoom = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComp"));
	Zoomzoomzoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);


	HitMarker = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	HitMarker->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	HitMarker->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitMarker->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	HitMarker->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
	//HitMarker->OnComponentBeginOverlap.AddDynamic(this, &APChar::OnOverlapBegin);
	

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsHandleComponent->LinearDamping = 200.f;
	PhysicsHandleComponent->LinearStiffness = 750.f;
	PhysicsHandleComponent->AngularDamping = 500.f;
	PhysicsHandleComponent->AngularStiffness = 1500.f;
	PhysicsHandleComponent->InterpolationSpeed = 50.f;
	PhysicsHandleComponent->bRotationConstrained = true;
	PhysicsHandleComponent->SetActive(true);
	PhysicsHandleComponent->Activate();
}

//Replicated the Team Variable 
void APChar::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APChar, Team);
}

// Called when the game starts or when spawned
void APChar::BeginPlay()
{
	Super::BeginPlay();
	
	SetTeam();
}

void APChar::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APChar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MoveForward", this, &APChar::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APChar::MoveRight);
	InputComponent->BindAxis("Turn", this, &APChar::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APChar::AddControllerPitchInput);
	InputComponent->BindAction("Jump", IE_Pressed, this, &APChar::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &APChar::OnStopJump);
	InputComponent->BindAction("LeftMouseFire", IE_Pressed, this, &APChar::FireTrace);
	InputComponent->BindAction("LeftMouseFire", IE_Released, this, &APChar::StopFire);

	InputComponent->BindAction("Pass", IE_Pressed, this, &APChar::PassBall);
	InputComponent->BindAction("Tab Menu", IE_Pressed, this, &APChar::OpenTab);
	InputComponent->BindAction("Tab Menu", IE_Released, this, &APChar::CloseTab);
	InputComponent->BindAxis("LookUpRate", this, &APChar::LookUpAtRate);
	InputComponent->BindAxis("TurnRate", this, &APChar::TurnAtRate);
	InputComponent->BindAction("IncX", IE_Pressed, this, &APChar::XSenseUP);
	InputComponent->BindAction("IncY", IE_Pressed, this, &APChar::YSenseUP);
	InputComponent->BindAction("DecX", IE_Pressed, this, &APChar::XSenseDown);
	InputComponent->BindAction("DecY", IE_Pressed, this, &APChar::YSenseDown);
	//sever test, ask mo if we can delete, press 5 to send to server
	InputComponent->BindAction("SendServerTest", IE_Released, this, &APChar::SendServerTest);


}

//
void APChar::SendServerTest()
{

	return;
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "debug SendServerTest");

	if (Role != ROLE_Authority)return;
	UClient* client = ConstructObject<UClient>(UClient::StaticClass());
	if (!client)return;
	bool sent;
	bool connect = client->Init();
	if (connect)
		 sent = client->SendHighScores();
	if (sent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Sent");
	}
	*/
}

void APChar::XSenseUP()
{
	APC* tempPC = Cast<APC>(this->Controller);
	tempPC->BaseTurnRate += 10;
}

void APChar::XSenseDown()
{
	APC* tempPC = Cast<APC>(this->Controller);
	tempPC->BaseTurnRate -= 10;
}

void APChar::YSenseUP()
{
	APC* tempPC = Cast<APC>(this->Controller);
	tempPC->BaseLookUpRate += 10;
}

void APChar::YSenseDown()
{
	APC* tempPC = Cast<APC>(this->Controller);
	tempPC->BaseLookUpRate -= 10;
}


void APChar::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APChar::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APChar::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	APC* tempPC = Cast<APC>(this->Controller);
	AddControllerYawInput(Rate * tempPC->BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APChar::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	APC* tempPC = Cast<APC>(this->Controller);
	AddControllerPitchInput(Rate * tempPC->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APChar::OnStartJump()
{
	bPressedJump = true;
}
void APChar::OnStopJump()
{
	bPressedJump = false;
}

void APChar::PassBall()
{
	APState* ThisState = Cast<APState>(this->PlayerState);
	if (!ThisState->hasBall)return;
	ServerPassBall();
}

void APChar::OpenTab()
{
	APC* MyController = Cast<APC>(this->Controller);
	MyController->OpenTab();
	BoxingCastTest();

}

void APChar::CloseTab()
{
	APC* MyController = Cast<APC>(this->Controller);
	MyController->CloseTab();
}

bool APChar::ServerPassBall_Validate()
{
	return true;
}

void APChar::ServerPassBall_Implementation()
{



	FVector Storeloc;
	FRotator StoreRot;
	GetController()->GetPlayerViewPoint(Storeloc, StoreRot);
	const FVector Start = Storeloc;
	const FVector End = Start + StoreRot.Vector() * 18000.0f;
	FHitResult HitData(ForceInit);
	FCollisionQueryParams Params;
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel2;
	Params.TraceTag = "Pass";
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitData, Start, End, CollisionChannel, Params))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, HitData.GetActor()->GetName());






		

		APChar*  PassedChar = Cast<APChar>(HitData.GetActor());
		if (!PassedChar)return;
		APState* MyState = Cast<APState>(this->PlayerState);
		APState* TheirState = Cast<APState>(PassedChar->PlayerState);

		TheirState->SetHasBall(true);// --uncomment
		MyState->SetHasBall(false);

		this->PlaySound(2);
		PassedChar->PlaySound(1);
		
		
		for (TActorIterator<APickUpBall> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			APickUpBall* tempBall = Cast<APickUpBall>(*ActorItr);
			if (!tempBall)return;

			tempBall->AttachToComponent(PassedChar->GetBallActor(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);






			AGS* GState = Cast<AGS>(UGameplayStatics::GetGameState(GetWorld()));

			GState->WhoHasTheBall = TheirState->GetTeam();

			//tempBall->MeshChange();

		}
	}

}

void APChar::PlaySound_Implementation(int32 input)
{
	switch (input)
	{
	case 1:
		//ObtainTheBall
		UGameplayStatics::SpawnSoundAttached(SoundBallGet, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		break;
	case 2://Lose the ball
		UGameplayStatics::SpawnSoundAttached(SoundBallLose, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		break;
	case 3://Death
		UGameplayStatics::SpawnSoundAttached(SoundDeath, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		break;
	case 4: //Respawn
		UGameplayStatics::SpawnSoundAttached(SoundRespawn, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		break;
	case 5://Hittsound
		UGameplayStatics::SpawnSoundAttached(SoundGiveHit, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);

		break;

	case 6: //Clock Ticking down 
		UGameplayStatics::SpawnSoundAttached(ClockTickDown, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		break;

	case 7: //Shoot Sound
		UGameplayStatics::SpawnSoundAttached(SoundShoot, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);

		break;


	default:
		break;
		

	}
}

void APChar::FireTrace()
{
	
	
	StartFire();
	GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this, &APChar::StartFire, 0.2, true);
	return;
		ServerCheckHit();
}

void APChar::StartFire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bang", true);
	PlaySound(7);
	FVector storageVec;
	FRotator storageRot;
	GetController()->GetPlayerViewPoint(storageVec, storageRot);
	const FVector TrailStart = LaserEmitParticle->GetComponentLocation();
	const FVector Start = storageVec;
	const FVector End = Start + storageRot.Vector()* 18000.0f;
	FHitResult HitData(ForceInit);
	FCollisionQueryParams Params;
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel2;
	Params.TraceTag = "Fire";
	Params.AddIgnoredActor(this);
	
	
	
	if (GetWorld()->LineTraceSingleByChannel(HitData, Start, End, CollisionChannel, Params))
	{
		
		
		ShowTrailTrace(TrailStart, End);
		
		APChar* victim = Cast<APChar>(HitData.GetActor());
		if (!victim)
		{
			Destructable(HitData);
			ShowHit(HitData.Location, 1);
			return;
		}

		APC* VictimsController = Cast<APC>(victim->Controller);
		APState* tempState = Cast<APState>(victim->PlayerState);
		APState* CurrentState = Cast<APState>(this->PlayerState);
		APC*  CurrentController = Cast<APC>(this->Controller);
		if (!CurrentState)return;
		if ((!tempState) || (tempState->Team.EqualToCaseIgnored(CurrentState->Team)))return;
		if (!CurrentController)return;
		CurrentController->middleman(1);
		VictimsController->middleman(2);
		VictimsController->Buzz();
		tempState->health -= 34;
		ShowHit(HitData.Location, 2);
		PlaySound(5);
		if (tempState->health <= 31)
		{

			tempState->Deaths++;
			CurrentState->Kills++;
			DeathEffects(victim);
			Death(victim);

		}
	}
	else
		ShowTrailTrace(TrailStart, End);

}

void APChar::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireRateHandle);
	FireRateHandle.Invalidate();
}






bool APChar::ServerCheckHit_Validate()
{
	return true;
}

void APChar::ServerCheckHit_Implementation()
{
	FVector storageVec;
	FRotator storageRot;
	GetController()->GetPlayerViewPoint(storageVec, storageRot);
	const FVector TrailStart = LaserEmitParticle->GetComponentLocation();
	const FVector Start = storageVec;
	const FVector End = Start + storageRot.Vector()* 18000.0f;
	FHitResult HitData(ForceInit);
	FCollisionQueryParams Params;
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel2;
	Params.TraceTag = "Fire";
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitData, Start, End, CollisionChannel, Params))
	{
		
		ShowTrailTrace(TrailStart, End);

		APChar* victim = Cast<APChar>(HitData.GetActor());
		if (!victim)
		{
			ShowHit(HitData.Location, 1);
			return;
		}
		
		APC* VictimsController = Cast<APC>(victim->Controller);
		APState* tempState = Cast<APState>(victim->PlayerState);
		APState* CurrentState = Cast<APState>(this->PlayerState);
		APC*  CurrentController = Cast<APC>(this->Controller);
		if (!CurrentState)return;
		if ((!tempState) || (tempState->Team.EqualToCaseIgnored(CurrentState->Team)))return;
		if (!CurrentController)return;
		CurrentController->middleman(1);
		VictimsController->middleman(2);
		VictimsController->Buzz();
		tempState->health -= 34;
		ShowHit(HitData.Location, 2);
		PlaySound(5);
		if (tempState->health <= 31)
		{
			
			tempState->Deaths++;
			CurrentState->Kills++;
			Death(victim);
		}
	}
	else
		ShowTrailTrace(TrailStart, End);


}

void APChar::ShowTrailTrace_Implementation(FVector Source, FVector Target)
{
	
	
	if (TraceEffect)
	{
			UParticleSystemComponent* beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, Source);
			if (beam)
			{
				beam->SetBeamSourcePoint(0, Source, 0);
				beam->SetBeamTargetPoint(0, Target, 0);

			}
	}
}



void APChar::ShowHit_Implementation(FVector hitLocation, int32 HitType)
{
	switch (HitType)
	{
	case 1:
		if (Explosion)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, hitLocation);
		break;
	case 2:
		if (HitExplosion)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitExplosion, hitLocation);
		break;
	default:
		break;
	}
}

void APChar::Death(APChar* player)
{
	//if we are server 
	if (Role == ROLE_Authority)
	{
		
		APState* tempState = Cast<APState>(player->PlayerState);
		if (tempState->hasBall)
		{
			for (TActorIterator<APickUpBall> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
				APickUpBall* tempBall = Cast<APickUpBall>(*ActorItr);
				if (tempBall)
				{
					USphereComponent* SphereComponent = Cast<USphereComponent>(tempBall->GetRootComponent());
					SphereComponent->SetSimulatePhysics(true);
					//tempBall->Target = NULL;
					tempState->hasBall = false;
					AGS* GState = Cast<AGS>(UGameplayStatics::GetGameState(GetWorld()));
					GState->WhoHasTheBall = FText::FromString("Dicks");
				}
			}
		}
		player->PlaySound(3);

		APC* tempController = Cast <APC>(player->Controller);
		if (bGameEnd == false)
		{
			tempController->middleman(4);
		}
		tempController->ServerSpawnSpectate();
		


	}
}

void APChar::FellOutOfWorld(const UDamageType & dmgType)
{
	Death(this);
}



void APChar::SetTeam()
{
	if (Team.EqualTo(FText::FromString("Red")))
	{
		this->GetMesh()->SetSkeletalMesh(RedMesh);
		this->GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
		RedParticle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
		Zoomzoomzoom->SetLightColor(FLinearColor(250.0, 0.0, 0.0, 1.0));
		Zoomzoomzoom->SetIntensity(5000.0f);
	}
	else if (Team.EqualTo(FText::FromString("Blue")))
	{
		this->GetMesh()->SetSkeletalMesh(BlueMesh);
		this->GetMesh()->SetRelativeRotation(FRotator(0.0, -90, 0.0));
		BlueParticle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
		Zoomzoomzoom->SetLightColor(FLinearColor(0.0, 0.0, 250.0, 1.0));
		Zoomzoomzoom->SetIntensity(5000.0f);
	}






}

void APChar::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	return;




}

void APChar::BoxingCastTest()
{


}