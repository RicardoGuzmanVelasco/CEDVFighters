// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CEDVFightersEnums.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CEDVFightersGameMode.generated.h"

UCLASS(MinimalAPI)
class ACEDVFightersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACEDVFightersGameMode();

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int Level;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int Wave;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int MaxWaves;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int Score;
	UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadWrite)
		int KilledShips;
	UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadWrite)
		int MaxLevelShips;

	UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadWrite)
		int KilledCannons;
	
	UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadWrite)
		EGamePhases GamePhase;
};



