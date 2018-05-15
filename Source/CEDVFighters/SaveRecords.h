#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveRecords.generated.h"

USTRUCT(BlueprintType)
struct FGameRecord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Score")
		int Points;
	UPROPERTY(BlueprintReadWrite, Category = "Score")
		int Level;
	UPROPERTY(BlueprintReadWrite, Category = "Score")
		int Wave;
	UPROPERTY(BlueprintReadWrite, Category = "Name")
		FString Name;

	FGameRecord(int Points = 0, int Level = 0, int Wave = 0, FString Name = "UNKNOWN") : Points(Points), Name(Name),
		Level(Level), Wave(Wave){}

	FORCEINLINE bool operator<(const FGameRecord &Other) const
	{
		if (Points == Other.Points)
			if (Level == Other.Level)
				return Wave < Other.Wave;
			else
				return Level < Other.Level;
		return Points < Other.Points;
	}
};


/**
 *
 */
UCLASS()
class CEDVFIGHTERS_API USaveRecords : public USaveGame
{
	GENERATED_BODY()

public:
	static const int MaxRecords = 5;

	UPROPERTY(VisibleAnywhere, Category = Records)
		TArray<FGameRecord> Easy;
	UPROPERTY(VisibleAnywhere, Category = Records)
		TArray<FGameRecord> Normal;
	UPROPERTY(VisibleAnywhere, Category = Records)
		TArray<FGameRecord> Hard;
	UPROPERTY(VisibleAnywhere, Category = Records)
		TArray<FGameRecord> Extreme;

	UPROPERTY(VisibleAnywhere, Category = Records)
		FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Records)
		uint32 UserIndex;

};
