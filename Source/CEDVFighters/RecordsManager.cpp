#include "RecordsManager.h"
#include "CEDVFightersEnums.h"
#include "CEDVFightersGameMode.h"
#include "CEDVFightersGameInstance.h"
#include "Algo/Reverse.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARecordsManager::ARecordsManager()
{
	//PrimaryActorTick.bCanEverTick = true;
	RecordsArray = {};
}



void ARecordsManager::LoadSaveInstance()
{
	LoadRecordsInstance = Cast<USaveRecords>(UGameplayStatics::CreateSaveGameObject(USaveRecords::StaticClass()));
	LoadRecordsInstance = Cast<USaveRecords>(UGameplayStatics::LoadGameFromSlot(LoadRecordsInstance->SaveSlotName, LoadRecordsInstance->UserIndex));

	LoadRecords();
}

void ARecordsManager::LoadRecords()
{
	UCEDVFightersGameInstance *gi = Cast<UCEDVFightersGameInstance>(GetGameInstance());
	switch (gi->SelectedDifficult) {
	case EDifficult::DIFF_Easy:
		RecordsArray = LoadRecordsInstance->Easy;
		break;
	case EDifficult::DIFF_Normal:
		RecordsArray = LoadRecordsInstance->Normal;
		break;
	case EDifficult::DIFF_Hard:
		RecordsArray = LoadRecordsInstance->Hard;
		break;
	case EDifficult::DIFF_Extreme:
		RecordsArray = LoadRecordsInstance->Extreme;
		break;
	}
	RecordsArray.Sort();
	Algo::Reverse(RecordsArray);
}

void ARecordsManager::ResetSaveInstance()
{
		LoadRecordsInstance->Easy.Empty(USaveRecords::MaxRecords);
		LoadRecordsInstance->Normal.Empty(USaveRecords::MaxRecords);
		LoadRecordsInstance->Hard.Empty(USaveRecords::MaxRecords);
		LoadRecordsInstance->Extreme.Empty(USaveRecords::MaxRecords);
	UGameplayStatics::SaveGameToSlot(LoadRecordsInstance, LoadRecordsInstance->SaveSlotName, LoadRecordsInstance->UserIndex);
}

void ARecordsManager::WriteSaveInstance()
{
	//RecordsArray.Sort();
	//Algo::Reverse(RecordsArray);
	UCEDVFightersGameInstance *gi = Cast<UCEDVFightersGameInstance>(GetGameInstance());
	switch (gi->SelectedDifficult) {
	case EDifficult::DIFF_Easy:
		LoadRecordsInstance->Easy = RecordsArray;
		break;
	case EDifficult::DIFF_Normal:
		LoadRecordsInstance->Normal = RecordsArray;
		break;
	case EDifficult::DIFF_Hard:
		LoadRecordsInstance->Hard = RecordsArray;
		break;
	case EDifficult::DIFF_Extreme:
		LoadRecordsInstance->Extreme = RecordsArray;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unknown difficult"));
		break;
	}
	UGameplayStatics::SaveGameToSlot(LoadRecordsInstance, LoadRecordsInstance->SaveSlotName, LoadRecordsInstance->UserIndex);
}

TArray<FGameRecord> ARecordsManager::GetRecords(EDifficult FromDifficult)
{
	TArray<FGameRecord> Array;
	switch (FromDifficult) {
	case EDifficult::DIFF_Easy:
		Array = LoadRecordsInstance->Easy;
		break;
	case EDifficult::DIFF_Normal:
		Array = LoadRecordsInstance->Normal;
		break;
	case EDifficult::DIFF_Hard:
		Array = LoadRecordsInstance->Hard;
		break;
	case EDifficult::DIFF_Extreme:
		Array = LoadRecordsInstance->Extreme;
		break;
	}
	Array.Sort();
	Algo::Reverse(Array);
	return Array;
}

void ARecordsManager::BeginPlay()
{
	Super::BeginPlay();

	LoadSaveInstance();
}

void ARecordsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARecordsManager::SaveNewRecord(FString Name, int Points, int Level, int Wave)
{
	auto Record = FGameRecord(Points, Level, Wave, Name);
	TArray<FGameRecord>* Records = &RecordsArray; //Copy avoiding overwrite problems.

	RecordsArray.Sort();
	Algo::Reverse(RecordsArray);

	if (Records->Num() < USaveRecords::MaxRecords)
		Records->Add(Record);
	else
		for (int i = 0; i < Records->Num(); i++)
			if (!(Record < RecordsArray[i]))
			{
				RecordsArray.RemoveAt(RecordsArray.Num() - 1);
				RecordsArray.Insert(Record, i - 1);
				RecordsArray.SetNum(USaveRecords::MaxRecords);
				break;
			}
	WriteSaveInstance();
}

bool ARecordsManager::IsRecord(int Points, int Level, int Wave)
{
	//Empty record spaces ready to fill.
	if (RecordsArray.Num() < USaveRecords::MaxRecords)
		return true;

	RecordsArray.Sort();
	Algo::Reverse(RecordsArray);

	auto Record = FGameRecord(Points, Level, Wave);
	for (int i = 0; i < RecordsArray.Num(); i++)
		if (!(Record < RecordsArray[i]))
			return true;

	return false;
}
