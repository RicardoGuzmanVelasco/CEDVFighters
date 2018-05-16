#include "RecordsManager.h"
#include "CEDVFightersEnums.h"
#include "CEDVFightersGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARecordsManager::ARecordsManager()
{
	//PrimaryActorTick.bCanEverTick = true;

}



void ARecordsManager::LoadSaveInstance()
{
	LoadRecordsInstance = Cast<USaveRecords>(UGameplayStatics::CreateSaveGameObject(USaveRecords::StaticClass()));
	LoadRecordsInstance = Cast<USaveRecords>(UGameplayStatics::LoadGameFromSlot(LoadRecordsInstance->SaveSlotName, LoadRecordsInstance->UserIndex));

	LoadRecords();
}

void ARecordsManager::LoadRecords()
{
	ACEDVFightersGameMode *gm = (ACEDVFightersGameMode *)UGameplayStatics::GetGameMode(this);

	if (gm == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GAMEMODE UNABLE TO FIND."))
		return;
	}

	switch (1) {
	case 1:
		RecordsArray = LoadRecordsInstance->Easy;
		break;
	case 2:
		RecordsArray = LoadRecordsInstance->Normal;
		break;
	case 3:
		RecordsArray = LoadRecordsInstance->Hard;
		break;
	case 4:
		RecordsArray = LoadRecordsInstance->Extreme;
		break;
	}
}

void ARecordsManager::ResetSaveInstance()
{
	LoadSaveInstance();
	RecordsArray.Empty(USaveRecords::MaxRecords);

	//To-Do: save clear.
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
