// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CEDVFightersPawn.h"
#include "CEDVFightersProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Components/InputComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName ACEDVFightersPawn::MoveForwardBinding("MoveForward");
const FName ACEDVFightersPawn::MoveRightBinding("MoveRight");
const FName ACEDVFightersPawn::FireForwardBinding("FireForward");
const FName ACEDVFightersPawn::FireRightBinding("FireRight");

ACEDVFightersPawn::ACEDVFightersPawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/MESHES/PlayerShip_Mesh.PlayerShip_Mesh"));
	// StaticMesh'/Game/MESHES/PlayerShip_Mesh.PlayerShip_Mesh'
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// ParticleSystem'/Game/FX/MotorFlame/FX_MotorFlame.FX_MotorFlame'
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_LeftFlame(TEXT("/Game/FX/MotorFlame/FX_MotorFlame.FX_MotorFlame"));
	LeftFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftFlame"));
	LeftFlame->Template = PS_LeftFlame.Object;
	LeftFlame->SetupAttachment(RootComponent);
	LeftFlame->RelativeLocation = FVector(-110.0f,-10.0f, 20.0f);
	LeftFlame->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
	LeftFlame->RelativeScale3D = FVector(8.0f, 8.0f, 8.0f);

	RightFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightFlame"));
	RightFlame->Template = PS_LeftFlame.Object;
	RightFlame->AttachToComponent(LeftFlame, FAttachmentTransformRules::KeepRelativeTransform);
	RightFlame->RelativeLocation = FVector(2.5f, 0.0f, 0.0f);
	RightFlame->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	RightFlame->RelativeScale3D = FVector(1.0f, 1.0f, 1.0f);

	SmallLeftFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmallLeftFlame"));
	SmallLeftFlame->Template = PS_LeftFlame.Object;
	SmallLeftFlame->SetupAttachment(RootComponent);
	SmallLeftFlame->RelativeLocation = FVector(-70.0f, -140.0f, 10.0f);
	SmallLeftFlame->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);
	SmallLeftFlame->RelativeScale3D = FVector(4.0f, 4.0f, 4.0f);

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	/*
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm
	*/


	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

void ACEDVFightersPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void ACEDVFightersPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
	
	// Create fire direction vector
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void ACEDVFightersPawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				World->SpawnActor<ACEDVFightersProjectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACEDVFightersPawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void ACEDVFightersPawn::ShotTimerExpired()
{
	bCanFire = true;
}

