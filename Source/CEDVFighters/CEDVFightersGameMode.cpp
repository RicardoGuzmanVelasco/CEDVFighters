// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CEDVFightersGameMode.h"
#include "CEDVFightersPawn.h"

ACEDVFightersGameMode::ACEDVFightersGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ACEDVFightersPawn::StaticClass();

	Level = 1;
	Wave = 1;
	MaxWaves = 1;
	Score = 0;
	MaxLevelShips = 0;
	KilledShips = 0;
	GamePhase = EGamePhases::GPHASE_Waves;
	KilledCannons = 0;
}

