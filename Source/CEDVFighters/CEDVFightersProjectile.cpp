// Copyright 1998-2018 Epic Games, Inc. All Rights Reserve

#include "CEDVFightersProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMesh.h"

ACEDVFightersProjectile::ACEDVFightersProjectile() 
{
	// Static reference to the mesh to use for the projectile
	//StaticMesh'/Game/MESHES/ProjectileMesh.ProjectileMesh'
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Game/MESHES/ProjectileMesh.ProjectileMesh"));
	
	// Create mesh component for the projectile sphere
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh0"));
	ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMesh->OnComponentHit.AddDynamic(this, &ACEDVFightersProjectile::OnHit);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &ACEDVFightersProjectile::OnOverlapBegin);

	RootComponent = ProjectileMesh;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	static ConstructorHelpers::FObjectFinder<UBlueprint> BPExplosion(TEXT("Blueprint'/Game/FX/2DExplosion/BP_2DExplosion.BP_2DExplosion'"));
	if (BPExplosion.Succeeded())
	{
		ExplosionClass = BPExplosion.Object->GeneratedClass;
	}

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ACEDVFightersProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, ExplosionClass->GetName());

	// Spawns explosion effect and destroy object and projectile
	if ((OtherActor != NULL) && (OtherActor != this))
	{
		if (OtherActor->ActorHasTag(TEXT("Enemy")))
		{
			const FVector explosionLocation = OtherActor->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f);
			const FRotator explosionRotator = FRotator(0.0f);
			GWorld->SpawnActor(ExplosionClass, &explosionLocation, &explosionRotator);
			OtherActor->Destroy();

			Destroy();
		}
	}
}

void ACEDVFightersProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("ProjectileTrigger")))
		Destroy();
}