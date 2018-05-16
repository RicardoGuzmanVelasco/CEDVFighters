#include "RecordsManager.h"
#include "CEDVFightersEnums.h"
#include "CEDVFightersGameMode.h"
#include "CEDVFightersGameInstance.h"
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
}

void ARecordsManager::ResetSaveInstance()
{
	//LoadSaveInstance();
	RecordsArray.Empty(USaveRecords::MaxRecords);

	WriteSaveInstance();
}

void ARecordsManager::WriteSaveInstance()
{
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

	auto Record = FGameRecord(Points, Level, Wave);
	for (int i = 0; i < RecordsArray.Num(); i++)
		if (!(Record < RecordsArray[i]))
			return true;

	return false;
}
