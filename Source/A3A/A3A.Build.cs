// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class A3A : ModuleRules
{ 
	public A3A(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "HeadMountedDisplay", "EnhancedInput", "NavigationSystem", "GameplayTasks" });
	}
}
