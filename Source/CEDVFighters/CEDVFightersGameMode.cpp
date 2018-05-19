// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CEDVFightersGameMode.h"
#include "CEDVFightersPawn.h"
#include "Kismet/GameplayStatics.h"

ACEDVFightersGameMode::ACEDVFightersGameMode() : Super()
{
	// set default pawn class to our character class
	DefaultPawnClass = ACEDVFightersPawn::StaticClass();

	Level = 1;
	Init();
}

void ACEDVFightersGameMode::Init()
{	
	Wave = 0;
	MaxWaves = 1;
	Score = 0;
	MaxLevelShips = 0;
	KilledShips = 0;
	GamePhase = EGamePhases::GPHASE_Waves;
	KilledCannons = 0;
}

void ACEDVFightersGameMode::ReConstructHordes()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), LevelGeneratorClass, FoundActors);

	if (FoundActors.IsValidIndex(0))
	{
		AActor* LevelGenerator = FoundActors[0];

		LevelGenerator->RerunConstructionScripts();
	}
}

