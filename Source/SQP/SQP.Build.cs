// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class SQP : ModuleRules
{
	public SQP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore",
			"GentleKit",
			"Niagara",
			"NetCore",
			"OnlineSubsystem", 
			"OnlineSubsystemUtils",
			"Sockets"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"SQP",
			"SQP/Variant_Platforming",
			"SQP/Variant_Combat",
			"SQP/Variant_Combat/AI",
			"SQP/Variant_SideScrolling",
			"SQP/Variant_SideScrolling/Gameplay",
			"SQP/Variant_SideScrolling/AI",
			Path.Combine(ModuleDirectory, "Lobby", "Public"),
			Path.Combine(ModuleDirectory, "Yeomin", "Public"),
			Path.Combine(ModuleDirectory, "DataAssets", "Public"),
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
