#include "RecordsManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARecordsManager::ARecordsManager()
{
	//PrimaryActorTick.bCanEverTick = true;

}



void ARecordsManager::LoadRecords()
{
	LoadRecordsInstance = Cast<USaveRecords>(UGameplayStatics::CreateSaveGameObject(USaveRecords::StaticClass()));
	LoadRecordsInstance = Cast<USaveRecords>(UGameplayStatics::LoadGameFromSlot(LoadRecordsInstance->SaveSlotName, LoadRecordsInstance->UserIndex));

	RecordsArray = GetRecordsArray();
}

void ARecordsManager::ResetRecords()
{
	LoadRecords();
	RecordsArray.Empty(USaveRecords::MaxRecords);

	//To-Do: save clear.
}

void ARecordsManager::BeginPlay()
{
	Super::BeginPlay();

	LoadRecords();
}

TArray<FGameRecord> ARecordsManager::GetRecordsArray()
{
	//To-Do: return according the difficult of gamemode.
	TArray<FGameRecord> Array;
	switch (1) {
	case 1:
		Array = LoadRecordsInstance->Easy;
		break;
	case 2:
		Array = LoadRecordsInstance->Normal;
		break;
	case 3:
		Array = LoadRecordsInstance->Hard;
		break;
	case 4:
		Array = LoadRecordsInstance->Extreme;
		break;
	}
	return Array;
}

void ARecordsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARecordsManager::SaveNewRecord(FString Name, int Points, int Level, int Wave)
{
	auto Record = FGameRecord(Points, Level, Wave, Name);
}

bool ARecordsManager::IsRecord(int Points, int Level, int Wave)
{
	//Empty record spaces ready to fill.
	if (RecordsArray.Num() < USaveRecords::MaxRecords)
		return true;

	auto Record = FGameRecord(Points, Level, Wave);
	for (int i = 0; i < RecordsArray.Num(); i++)
		if (!(Record < RecordsArray[i]))
			return true;

	return false;
}
