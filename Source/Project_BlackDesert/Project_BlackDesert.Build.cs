// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_BlackDesert : ModuleRules
{
	public Project_BlackDesert(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks", 
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

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] {
            "Project_BlackDesert",
            "Project_BlackDesert/TP_ThirdPerson/Variant_Platforming",
            "Project_BlackDesert/TP_ThirdPerson/Variant_Platforming/Animation",
            "Project_BlackDesert/TP_ThirdPerson/Variant_Combat",
            "Project_BlackDesert/TP_ThirdPerson/Variant_Combat/AI",
            "Project_BlackDesert/TP_ThirdPerson/Variant_Combat/Animation",
            "Project_BlackDesert/TP_ThirdPerson/Variant_Combat/Gameplay",
            "Project_BlackDesert/TP_ThirdPerson/Variant_Combat/Interfaces",
            "Project_BlackDesert/TP_ThirdPerson/Variant_Combat/UI",
            "Project_BlackDesert/TP_ThirdPerson/Variant_SideScrolling",
            "Project_BlackDesert/TP_ThirdPerson/Variant_SideScrolling/AI",
            "Project_BlackDesert/TP_ThirdPerson/Variant_SideScrolling/Gameplay",
            "Project_BlackDesert/TP_ThirdPerson/Variant_SideScrolling/Interfaces",
            "Project_BlackDesert/TP_ThirdPerson/Variant_SideScrolling/UI"
        });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
