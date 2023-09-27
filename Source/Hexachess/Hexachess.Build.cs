// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Hexachess : ModuleRules
{
	public Hexachess(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        bLegacyPublicIncludePaths = false;

		PublicIncludePaths.AddRange(new string[] { "Hexachess" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Json", "JsonUtilities", "HTTP", "GeometryCollectionEngine"});

		PrivateDependencyModuleNames.AddRange(new string[] { "Hexachess" });
	}
}
