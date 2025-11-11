// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UwsToyCppTP : ModuleRules
{
	public UwsToyCppTP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"ProceduralMeshComponent" });

		if(Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
	}
}
