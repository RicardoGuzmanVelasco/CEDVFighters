#include "SaveRecords.h"


USaveRecords::USaveRecords()
{
	Easy.Empty(USaveRecords::MaxRecords);
	Normal.Empty(USaveRecords::MaxRecords);
	Hard.Empty(USaveRecords::MaxRecords);
	Extreme.Empty(USaveRecords::MaxRecords);

	SaveSlotName = TEXT("DefaultSlot");
	UserIndex = 0;
}