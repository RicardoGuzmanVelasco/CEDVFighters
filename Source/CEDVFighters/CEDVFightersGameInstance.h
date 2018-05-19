// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CEDVFightersEnums.h"
#include "CEDVFightersGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CEDVFIGHTERS_API UCEDVFightersGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	EDifficult SelectedDifficult;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int CurrentScore;
	
	UCEDVFightersGameInstance();

};
