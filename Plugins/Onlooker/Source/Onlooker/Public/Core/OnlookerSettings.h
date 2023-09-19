#pragma once

#include <Engine/DeveloperSettings.h>

#include "OnlookerSettings.generated.h"


UCLASS(config = EditorPerProjectUserSettings, meta = (DisplayName = "Onlooker Plugin"))
class ONLOOKER_API UOnlookerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UOnlookerSettings()
	{
		CategoryName = TEXT("Plugins");
		SectionName = TEXT("Onlooker Plugin");
	}
};
