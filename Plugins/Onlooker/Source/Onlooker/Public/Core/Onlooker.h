#pragma once

#include <CoreMinimal.h>
#include <Modules/ModuleInterface.h>

#include "OnlookerSettings.h"

class FOnlookerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	UOnlookerSettings* OnlookerSettings = nullptr;
	FString PluginDirectory;
	FString GlobalSettingsFile;

	void ReloadConfiguration(UObject* Object, struct FPropertyChangedEvent& Property);
};
