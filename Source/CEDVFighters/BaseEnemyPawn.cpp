// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemyPawn.h"

#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h"
#include "Components/ArrowComponent.h"

#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "EngineMinimal.h"
#include "CEDVFightersGameMode.h"



// Sets default values
ABaseEnemyPawn::ABaseEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneComponent;

	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	ShipMeshComponent->SetupAttachment(RootComponent);
	ShipMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ShipMeshComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));

	ShipMeshComponent->OnComponentHit.AddDynamic(this, &ABaseEnemyPawn::OnHit);
	ShipMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemyPawn::OnOverlapBegin);

	ProjOffsetComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjOffset"));
	ProjOffsetComponent->SetupAttachment(RootComponent);
	ProjOffsetComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	PawnMovement->Velocity = FVector(-200.0f, 0.0f, 0.0f);

	//Blueprint'/Game/PAWNS/BP_PlayerProj_Normal.BP_PlayerProj_Normal'
	static ConstructorHelpers::FObjectFinder<UBlueprint> BPProjectile(TEXT("Blueprint'/Game/PAWNS/BP_PlayerProj_Normal.BP_PlayerProj_Normal'"));
	if (BPProjectile.Succeeded())
	{
		ProjectileClass = BPProjectile.Object->GeneratedClass;
	}

	CanShot = false;
	ApproachComplete = false;
	MinShotRate = 1.0;
	MaxShotRate = 2.0;
	ScorePoints = 100;
	IsLeader = false;
	BackDestroyed = false;

	this->Tags.AddUnique(FName("Enemy"));
}

// Called when the game starts or when spawned
void ABaseEnemyPawn::BeginPlay()
{
	Super::BeginPlay();

	const float ShotRate = FMath::FRandRange(MinShotRate, MaxShotRate);

	GetWorldTimerManager().SetTimer(TH_ShotTimer, this, &ABaseEnemyPawn::Shot, ShotRate, true);
		
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "actualizado");
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "actualizado2");
}

void ABaseEnemyPawn::EndPlay(const EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
}

// Called every frame
void ABaseEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApproachMove(DeltaTime);
}

void ABaseEnemyPawn::Destroyed()
{
	ACEDVFightersGameMode *gm = (ACEDVFightersGameMode *)UGameplayStatics::GetGameMode(this);

	if (gm != nullptr)
	{
		if (!BackDestroyed)
			gm->Score += ScorePoints;
		gm->KilledShips++;
	}
}
void ABaseEnemyPawn::ApproachMove(float DeltaSeconds)
{
	if (ApproachComplete) return;

	FRotator BaseRotation = this->GetBaseAimRotation();
	FVector DeltaMovement = DeltaSeconds * PawnMovement->Velocity;

	PawnMovement->MoveUpdatedComponent(DeltaMovement, BaseRotation, true);
}

void ABaseEnemyPawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this))
	{
		
	}
}

void ABaseEnemyPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("HordeTrigger")))
	{
		ApproachComplete = true;
		CanShot = true;
		this->SpawnDefaultController();

		
	} 
	else if (IsLeader && OtherActor->ActorHasTag(TEXT("WaveTrigger")))
	{
		ACEDVFightersGameMode *gm = (ACEDVFightersGameMode *)UGameplayStatics::GetGameMode(this);

		if (gm != nullptr)
		{
			gm->Wave++;
			IsLeader = false;
		}
	}
	else if (OtherActor->ActorHasTag(TEXT("BackTrigger")))
	{
		BackDestroyed = true;
		Destroy();
	}
}

void ABaseEnemyPawn::Shot()
{
	if (!CanShot) return;
	auto playerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	const FVector ProjectileLocation = ProjOffsetComponent->GetComponentLocation();
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, ProjectileLocation.ToString());

	const FVector TargetVector = (playerPawn->GetActorLocation() - ProjectileLocation);
	const FRotator ProjectileRotation = TargetVector.Rotation();

	const FTransform ProjectileTransform = FTransform(ProjectileRotation, ProjectileLocation);
	AActor* projectile = GWorld->SpawnActor(ProjectileClass, &ProjectileTransform);
	projectile->SetActorScale3D(FVector(1.0f));

	UGameplayStatics::PlaySoundAtLocation(this, FireSound, ProjectileLocation);
}


