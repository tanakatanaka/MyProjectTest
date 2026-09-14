// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProjectTest : ModuleRules
{
	public MyProjectTest(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MyProjectTest",
			"MyProjectTest/Variant_Platforming",
			"MyProjectTest/Variant_Platforming/Animation",
			"MyProjectTest/Variant_Combat",
			"MyProjectTest/Variant_Combat/AI",
			"MyProjectTest/Variant_Combat/Animation",
			"MyProjectTest/Variant_Combat/Gameplay",
			"MyProjectTest/Variant_Combat/Interfaces",
			"MyProjectTest/Variant_Combat/UI",
			"MyProjectTest/Variant_SideScrolling",
			"MyProjectTest/Variant_SideScrolling/AI",
			"MyProjectTest/Variant_SideScrolling/Gameplay",
			"MyProjectTest/Variant_SideScrolling/Interfaces",
			"MyProjectTest/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
