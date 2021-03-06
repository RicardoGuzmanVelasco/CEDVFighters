// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CEDVFightersEnums.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CEDVFightersPawn.generated.h"

UCLASS(Blueprintable)
class ACEDVFightersPawn : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	UPROPERTY(Category = Effect, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* LeftFlame; 
	UPROPERTY(Category = Effect, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* RightFlame;
	UPROPERTY(Category = Effect, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* SmallLeftFlame;
	UPROPERTY(Category = Effect, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* SmallRightFlame;

	TSubclassOf<class AActor> ExplosionClass;

public:
	ACEDVFightersPawn();

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite )
	FVector GunOffset;
	
	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	/* Max health for player */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float Health;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	// Screen margin to limit movements, in percentage
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float ScreenMarginPct;

	/* No taking damage */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	bool GodMode;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	ECameraModeEnum CameraMode;

	UPROPERTY()
		class ARecordsManager *RecordsMgr;

	UPROPERTY()
		class ACEDVFightersGameMode *GMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wFloatingRecords;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wGameOver;

	UFUNCTION()
	void HasDied();
	UFUNCTION()
	void SetRecords();

	// Begin Actor Interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	/* Handler for the fire timer expiry */
	void ShotTimerExpired();
	bool MoveOutScreen(const FVector &movement) const;
	void ScaleFlames(const float forwardValue);

	float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser);

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireForwardBinding;
	static const FName FireRightBinding;

private:

	/* Flag to control firing  */
	uint32 bCanFire : 1;
	uint32 bIsDead : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;
	FTimerHandle TimerHandle_ExplosionFinished;

public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
};

