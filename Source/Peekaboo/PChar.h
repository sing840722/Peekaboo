// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PChar.generated.h"

UCLASS()
class PEEKABOO_API APChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APChar();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION()
		void MoveForward(float Val);
	//handles strafing
	UFUNCTION()
		void MoveRight(float Val);
	//sets jump flag when key is pressed

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);



	//Ask mo if we can delete, testing sending to server lol
	void SendServerTest();

	void XSenseUP();

	void XSenseDown();
	void YSenseUP();

	void YSenseDown();

	UFUNCTION()
		void OnStartJump();
	//clears jump flag when key is released
	UFUNCTION()
		void OnStopJump();
	/************************************************************************/
	/* Ball passing                                                         */
	/************************************************************************/
	UFUNCTION()
		void PassBall();

	UFUNCTION(Server, withValidation, Reliable)
		void ServerPassBall();

	/************************************************************************/
	/* Opening the Tab                                                      */
	/************************************************************************/
	UFUNCTION()
		void OpenTab();
	UFUNCTION()
		void CloseTab();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WTabHolder;

	UUserWidget* wTab;
	
	/************************************************************************/
	/* Sound Effects                                                        */
	/************************************************************************/
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundGiveHit;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundDeath;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundBallGet;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundBallLose;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundRespawn;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundShoot;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* ClockTickDown;
	
	UFUNCTION(Client, Reliable)
		void PlaySound(int32 input);

	/************************************************************************/
	/* Camera comp                                                          */
	/************************************************************************/
	UPROPERTY(EditAnywhere)
		UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	UFUNCTION()
		void FireTrace();

	//------------------Particle
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* LaserEmitParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* HitExplosion;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* TraceEffect;

	UFUNCTION(NetMulticast, Unreliable)
		void ShowHit(FVector hitLocation, int32 HitType);

	UFUNCTION(NetMulticast, Unreliable)
		void ShowTrailTrace(FVector Source, FVector Target);
	

	//APChar* temp;
	UFUNCTION(Server, WithValidation, Reliable)
		void ServerCheckHit();

	void Death(APChar* player);

	UPROPERTY(EditAnywhere, Category = "BallPosition")
		UStaticMeshComponent* BallAddLocation;

	void GMRespawn();

	//UFUNCTION(Server, Reliable, WithValidation)
		//void ServerGMRespawn();

	
		UFUNCTION(BlueprintImplementableEvent)
		void Destructable(FHitResult Target);


		
		UFUNCTION(BlueprintImplementableEvent)
			void DeathEffects(APChar* Target);

		virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

		FTimerDelegate TimerDel;
		FTimerHandle TimerHandle;
		bool bGameEnd;




private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
		int32 health;

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Teams")
		FText Team;
	void SetTeam();

	UPROPERTY(EditAnywhere, Category = "ShootBox")
		USphereComponent* HitMarker;



private:
	UPROPERTY(EditDefaultsOnly, Category = "RedMesh")
	USkeletalMesh* RedMesh;

	UPROPERTY(EditDefaultsOnly, Category = "BluesMesh")
	USkeletalMesh* BlueMesh;

public:
	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystemComponent* RedParticle;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystemComponent* BlueParticle;
	UPROPERTY(EditAnywhere, Category = "PointLight")
		UPointLightComponent* Zoomzoomzoom;

	FORCEINLINE UStaticMeshComponent* GetBallActor() const { return BallAddLocation; };





	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void BoxingCastTest();

	UPhysicsHandleComponent* PhysicsHandleComponent;


	void StartFire();
	void StopFire();

private:
	//UFUNCTION()
	//void DestroyActor(APChar* player);

	//FTimerDelegate TimerDel;
	FTimerHandle FireRateHandle;


	

};
