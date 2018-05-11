// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CEDVFightersGameMode.generated.h"

UCLASS(MinimalAPI)
class ACEDVFightersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACEDVFightersGameMode();

	UPROPERTY(Category = GameMode, EditAnywhere, BlueprintReadWrite)
		int Level;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int Wave;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int MaxWaves;
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		int Score;
};



