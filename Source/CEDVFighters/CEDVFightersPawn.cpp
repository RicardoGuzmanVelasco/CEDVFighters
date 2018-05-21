// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CEDVFightersPawn.h"
#include "CEDVFightersProjectile.h"
#include "RecordsManager.h"
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
#include "Blueprint/UserWidget.h"
#include "EngineMinimal.h"
#include "CEDVFightersGameMode.h"
#include "CEDVFightersEnums.h"

const FName ACEDVFightersPawn::MoveForwardBinding("MoveForward");
const FName ACEDVFightersPawn::MoveRightBinding("MoveRight");
const FName ACEDVFightersPawn::FireForwardBinding("FireForward");


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
	LeftFlame->RelativeLocation = FVector(-110.0f, -10.0f, 20.0f);
	LeftFlame->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
	LeftFlame->RelativeScale3D = FVector(4.0f, 6.0f, 4.0f);

	RightFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightFlame"));
	RightFlame->Template = PS_LeftFlame.Object;
	RightFlame->SetupAttachment(RootComponent);
	//RightFlame->AttachToComponent(LeftFlame, FAttachmentTransformRules::KeepRelativeTransform);
	RightFlame->RelativeLocation = FVector(-110.0f, 10.0f, 20.0f);
	RightFlame->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
	RightFlame->RelativeScale3D = FVector(4.0f, 6.0f, 4.0f);

	SmallLeftFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmallLeftFlame"));
	SmallLeftFlame->Template = PS_LeftFlame.Object;
	SmallLeftFlame->SetupAttachment(RootComponent);
	SmallLeftFlame->RelativeLocation = FVector(-70.0f, -140.0f, 10.0f);
	SmallLeftFlame->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
	SmallLeftFlame->RelativeScale3D = FVector(3.0f, 4.0f, 3.0f);

	SmallRightFlame = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmallRightFlame"));
	SmallRightFlame->Template = PS_LeftFlame.Object;
	SmallRightFlame->SetupAttachment(RootComponent);
	SmallRightFlame->RelativeLocation = FVector(-70.0f, 140.0f, 10.0f);
	SmallRightFlame->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
	SmallRightFlame->RelativeScale3D = FVector(3.0f, 4.0f, 3.0f);

	// Cache our sound effect
	//SoundWave'/Game/SOUND/GAME/DisparoLaser2.DisparoLaser2'
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/SOUND/GAME/DisparoLaser2.DisparoLaser2"));
	FireSound = FireAudio.Object;

	//Blueprint'/Game/FX/2DExplosion/BP_BigExplosion.BP_BigExplosion'
	static ConstructorHelpers::FObjectFinder<UBlueprint> BPExplosion(TEXT("Blueprint'/Game/FX/2DExplosion/BP_BigExplosion.BP_BigExplosion'"));
	if (BPExplosion.Succeeded())
	{
		ExplosionClass = BPExplosion.Object->GeneratedClass;
	}

	// Movement
	MoveSpeed = 1500.0f;
	// Weapon
	GunOffset = FVector(140.f, 0.f, 0.f);
	FireRate = 0.05f;
	bCanFire = true;
	bIsDead = false;
	ScreenMarginPct = 5.0f;

	Health = MaxHealth = 100.0f;
	GodMode = false;

	CameraMode = ECameraModeEnum::CM_TopCamera;

	this->Tags.AddUnique(FName("Player"));
	
}

void ACEDVFightersPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
}


void ACEDVFightersPawn::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARecordsManager::StaticClass(), FoundActors);

	if (FoundActors.IsValidIndex(0))
		RecordsMgr = (ARecordsManager*)FoundActors[0];

	GMode = (ACEDVFightersGameMode *)UGameplayStatics::GetGameMode(this);
	
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

	//GEngine->AddOnScreenDebugMessage(-1, 0.50f, FColor::Red, FString::SanitizeFloat(ForwardValue));
	ScaleFlames(ForwardValue);

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
	if (bCanFire == true && GMode->GamePhase != EGamePhases::GPHASE_BossExpanding)
	{
		// If we are pressing fire stick in a direction
		//if (FireDirection.SizeSquared() > 0.0f)
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

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ACEDVFightersPawn::ShotTimerExpired, FireRate);
		}
	}
}

void ACEDVFightersPawn::ShotTimerExpired()
{
	bCanFire = true;
}

bool ACEDVFightersPawn::MoveOutScreen(const FVector &movement) const
{

	if (CameraMode != ECameraModeEnum::CM_TopCamera)
		return false;

	const FVector2D ScreenSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D ScreenMargin = ScreenMarginPct * 0.01f * ScreenSize;

	const FVector WorldPlayerPosition = GetActorLocation() + movement;

	FVector2D ScreenPlayerPosition;
	UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0),
		WorldPlayerPosition, ScreenPlayerPosition);

	//GEngine->AddOnScreenDebugMessage(-1, 0.50f, FColor::Red, ScreenPlayerPosition.ToString());

	if (ScreenPlayerPosition.X < ScreenMargin.X || ScreenPlayerPosition.X > ScreenSize.X - ScreenMargin.X ||
		ScreenPlayerPosition.Y < ScreenMargin.Y || ScreenPlayerPosition.Y > ScreenSize.Y - ScreenMargin.X)
		return true;

	return false;
}

void ACEDVFightersPawn::ScaleFlames(const float forwardValue)
{
	FVector flameScale;
	FVector smallFlameScale;

	switch ((int)forwardValue)
	{
	case 0:
		flameScale = FVector(4.0f, 6.0f, 4.0f);
		smallFlameScale = FVector(3.0f, 4.0f, 3.0f);
		break;
	case 1:
		flameScale = FVector(8.0f, 12.0f, 8.0f);
		smallFlameScale = FVector(6.0f, 8.0f, 6.0f);
		break;
	case -1:
		flameScale = FVector(2.0f, 3.0f, 2.0f);
		smallFlameScale = FVector(2.0f, 2.0f, 2.0f);
		break;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 0.050f, FColor::Blue, FString::FromInt((int)forwardValue));

	/*RightFlame->SetWorldScale3D(flameScale);
	LeftFlame->SetWorldScale3D(flameScale);
	SmallRightFlame->SetWorldScale3D(smallFlameScale);
	SmallLeftFlame->SetWorldScale3D(smallFlameScale);*/

	RightFlame->SetRelativeScale3D(flameScale);
	LeftFlame->SetRelativeScale3D(flameScale);
	SmallRightFlame->SetRelativeScale3D(smallFlameScale);
	SmallLeftFlame->SetRelativeScale3D(smallFlameScale);

	//RightFlame->RelativeScale3D = flameScale;
	//LeftFlame->RelativeScale3D = flameScale;
	//SmallRightFlame->RelativeScale3D = smallFlameScale;
	//SmallLeftFlame->RelativeScale3D = smallFlameScale;
}

float ACEDVFightersPawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, class AActor* DamageCauser)
{
	if (!GodMode)
		Health -= Damage;

	//To-Do: if Health <= 0, die.
	if (Health <= 0)
		this->HasDied();


	return Damage;
}

void ACEDVFightersPawn::HasDied()
{
	if (RecordsMgr == nullptr)
		return;

	if (GMode == nullptr)
		return;

	if (bIsDead)
		return;

	const FVector explosionLocation = GetActorLocation();
	const FRotator explosionRotator = FRotator(0.0f);
	
	GWorld->SpawnActor(ExplosionClass, &explosionLocation, &explosionRotator);

	bIsDead = true;
	this->SetActorHiddenInGame(true);
	
	GetWorldTimerManager().SetTimer(TimerHandle_ExplosionFinished, this, &ACEDVFightersPawn::SetRecords, 1.5, false);
}

void ACEDVFightersPawn::SetRecords()
{
	//GWorld->GetTimerManager().SetTimer(TimerHandle_ExplosionFinished, this, &ACEDVFightersPawn::SetRecords, -1.0);

	UGameplayStatics::SetGamePaused(this, true);

	int Score = GMode->Score;
	int	Level = GMode->Level;
	int Wave = GMode->Wave;

	FString p = FString::FromInt(Score) + " - " + FString::FromInt(Level) + " - " + FString::FromInt(Wave);

	if (RecordsMgr->IsRecord(Score, Level, Wave))
	{
		if (wFloatingRecords)
		{
			UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), wFloatingRecords);
			if (widget)
				widget->AddToViewport();
		}
	}
	else
	{
		if (wGameOver)
		{
			UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), wGameOver);
			if (widget)
				widget->AddToViewport();
		}
	}
}
