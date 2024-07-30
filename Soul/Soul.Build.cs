// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Soul : ModuleRules
{
	public Soul(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] { "Soul" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PublicDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara" });
		PrivateDependencyModuleNames.AddRange(new string[] { "MotionTrajectory" });
		PrivateDependencyModuleNames.AddRange(new string[] { "PhysicsCore" });
	}
}