// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseEnemyPawn.generated.h"

UCLASS()
class CEDVFIGHTERS_API ABaseEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UFUNCTION()
	void ApproachMove(float DeltaSeconds);

	UFUNCTION()
	void Shot();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	class USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	class UStaticMeshComponent* ShipMeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default, meta=(ToolTip = "Setup location by hand!!"))
	class UArrowComponent *ProjOffsetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	class UFloatingPawnMovement *PawnMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		class USoundBase *FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		bool CanShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		bool ApproachComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		float MinShotRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		float MaxShotRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		int ScorePoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		bool IsLeader;

	TSubclassOf<class AActor> ProjectileClass;

	FTimerHandle TH_ShotTimer;
	bool BackDestroyed;
};
