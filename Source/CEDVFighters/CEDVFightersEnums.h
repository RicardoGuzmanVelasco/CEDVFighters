#pragma once

#include "CoreMinimal.h"

// Enum for camera modes
UENUM(BlueprintType)
enum class ECameraModeEnum : uint8
{
	CM_TopCamera UMETA(DisplayName = "TopCamera"),
	CM_Perspective UMETA(DisplayName = "Perspective"),
	CM_BackCamera UMETA(DisplayName = "BackCamera")
};

// Enum for game phases
UENUM(BlueprintType)
enum class EGamePhases : uint8
{
	GPHASE_Waves UMETA(DisplayName = "Phase: Waves"),
	GPHASE_BossSpawn UMETA(DisplayName = "Phase: Boss Spawn"),
	GPHASE_BossAppproach UMETA(DisplayName = "Phase: Boss Approach"),
	GPHASE_BossCombat UMETA(DisplayName = "Phase: Boss Combat"),
	GPHASE_BossExpand UMETA(DisplayName = "Phase: Boss Expanded"),
	GPHASE_BossKilled UMETA(DisplayName = "Phase: Boss Killed")
};

UENUM(BlueprintType)
enum class EDifficult : uint8
{
	DIFF_Easy UMETA(DisplayName = "Difficult: Easy"),
	DIFF_Normal UMETA(DisplayName = "Difficult: Normal"),
	DIFF_Hard UMETA(DisplayName = "Difficult: Hard"),
	DIFF_Extreme UMETA(DisplayName = "Difficult: Extreme")
};