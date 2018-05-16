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
	EDifficult SelectedDifficult;
	
	
	UCEDVFightersGameInstance();

};
