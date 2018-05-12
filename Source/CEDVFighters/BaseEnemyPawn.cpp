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




// Sets default values
ABaseEnemyPawn::ABaseEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	ProjOffset = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjOffset"));
	ProjOffset->SetRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	//UFloatingPawnMovement
}

// Called when the game starts or when spawned
void ABaseEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



