// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Starfire : ModuleRules
{
	public Starfire(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput" ,
			"AIModule",
			"NavigationSystem",
			"GameplayTags"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UMG", 
			"Slate", 
			"SlateCore", 
			"ApplicationCore", 
			"DebugSystem", 
			"ObjectExtensions"
		});

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new string[]
			{
				"UnrealEd"
			});
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
