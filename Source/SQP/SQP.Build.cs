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
			//UI
			"UMG",
			"Slate",
			"SlateCore",
			//커스텀
			"GentleKit",
			//나이아가라
			"Niagara",
			//멀티플레이
			"NetCore",
			"OnlineSubsystem", 
			"OnlineSubsystemUtils",
			"Sockets",
			//클립보드
			"ApplicationCore", "Synthesis",
			"HTTP",
			"ImageWrapper",
			"Json",
			"JsonUtilities"
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
			
			//디폴트
			Path.Combine(ModuleDirectory, "Default", "Public"),
			
			//로비
			Path.Combine(ModuleDirectory, "Lobby/Default", "Public"),
			Path.Combine(ModuleDirectory, "Lobby/Widget", "Public"),
			
			//메인
			Path.Combine(ModuleDirectory, "Main/Default", "Public"),
			Path.Combine(ModuleDirectory, "Main/Widget", "Public"),
			
			//페인트룸
			Path.Combine(ModuleDirectory, "PaintRoom/Default", "Public"),
			Path.Combine(ModuleDirectory, "PaintRoom/Save", "Public"),
			Path.Combine(ModuleDirectory, "PaintRoom/Widget", "Public"),
			
			Path.Combine(ModuleDirectory, "UI", "Public"),
			Path.Combine(ModuleDirectory, "Yeomin", "Public"),
			Path.Combine(ModuleDirectory, "DataAssets", "Public"),
			Path.Combine(ModuleDirectory, "PaintGaming", "Public"),
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
