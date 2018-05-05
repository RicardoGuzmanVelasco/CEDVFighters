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
#include "EngineMinimal.h"

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
	//LeftFlame->AttachToComponent(ShipMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LeftFlame->SetupAttachment(RootComponent);
	LeftFlame->RelativeLocation = FVector(-110.0f,-10.0f, 20.0f);
	LeftFlame->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
	LeftFlame->RelativeScale3D = FVector(8.0f, 12.0f, 8.0f);

	RightFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightFlame"));
	RightFlame->Template = PS_LeftFlame.Object;
	RightFlame->SetupAttachment(RootComponent);
	//RightFlame->AttachToComponent(LeftFlame, FAttachmentTransformRules::KeepRelativeTransform);
	RightFlame->RelativeLocation = FVector(-110.0f, 10.0f, 20.0f);
	RightFlame->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
	RightFlame->RelativeScale3D = FVector(8.0f, 12.0f, 8.0f);


	SmallLeftFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmallLeftFlame"));
	SmallLeftFlame->Template = PS_LeftFlame.Object;
	SmallLeftFlame->SetupAttachment(RootComponent);
	SmallLeftFlame->RelativeLocation = FVector(-70.0f, -140.0f, 10.0f);
	SmallLeftFlame->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
	SmallLeftFlame->RelativeScale3D = FVector(6.0f, 8.0f, 6.0f);

	SmallRightFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmallRightFlame"));
	SmallRightFlame->Template = PS_LeftFlame.Object;
	SmallRightFlame->SetupAttachment(RootComponent);
	SmallRightFlame->RelativeLocation = FVector(-70.0f, 140.0f, 10.0f);
	SmallRightFlame->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
	SmallRightFlame->RelativeScale3D = FVector(6.0f, 8.0f, 6.0f);

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Movement
	MoveSpeed = 1500.0f;
	// Weapon
	GunOffset = FVector(140.f, 0.f, 0.f);
	FireRate = 0.05f;
	bCanFire = true;
	ScreenMarginPct = 10.0f;
}

void ACEDVFightersPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	//PlayerInputComponent->BindAxis(FireRightBinding);
}


void ACEDVFightersPawn::BeginPlay() 
{
	Super::BeginPlay();

	ScreenSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ScreenMargin = ScreenMarginPct * 0.01f * ScreenSize;
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ScreenLimits.ToString());
}


void ACEDVFightersPawn::Tick(float DeltaSeconds)
{
	// Shot
	const float FireValue = GetInputAxisValue(FireForwardBinding);
	if (FireValue > 0.0f)
	{
		// Try and fire a shot
		FireShot(FVector(1.0f, 0.0f, 0.0f));
	}

	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	if (MoveOutScreen(Movement))
		return;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		const FRotator NewRotation = RootComponent->GetComponentRotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

	//if (RightValue != 0.0f)
	
		const FRotator NewRotation = FRotator(0.0f, 0.0f, FMath::Sign(RightValue) * 30.0f);
		//RootComponent->SetRelativeRotation(NewRotation);
		SetActorRelativeRotation(NewRotation);

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

bool ACEDVFightersPawn::MoveOutScreen(const FVector &movement) const
{
	FVector WorldPlayerPosition = GetActorLocation() + movement;
	
	FVector2D ScreenPlayerPosition;
	UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0),
		WorldPlayerPosition, ScreenPlayerPosition);
	
	//GEngine->AddOnScreenDebugMessage(-1, 0.50f, FColor::Red, ScreenPlayerPosition.ToString());

	if (ScreenPlayerPosition.X < 0 + ScreenMargin.X || ScreenPlayerPosition.X > ScreenSize.X - ScreenMargin.X ||
		ScreenPlayerPosition.Y < 0 + ScreenMargin.Y || ScreenPlayerPosition.Y > ScreenSize.Y - ScreenMargin.X)
	return true;

	return false;
}

