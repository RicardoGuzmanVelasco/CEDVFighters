// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveRecords.h"
#include "RecordsManager.generated.h"

UCLASS()
class CEDVFIGHTERS_API ARecordsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ARecordsManager();
	void LoadSaveInstance();
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadRecords();
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
		void ResetSaveInstance();
	void WriteSaveInstance();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	TArray<FGameRecord> GetRecords(EDifficult FromDifficult);

protected:
	virtual void BeginPlay() override;

	TArray<FGameRecord> RecordsArray;
	UPROPERTY()
		class USaveRecords* LoadRecordsInstance;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void SaveNewRecord(FString Name, int Points, int Level, int Wave);
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	bool IsRecord(int Points, int Level, int Wave);
	
};
