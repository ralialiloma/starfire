using UnrealBuildTool;

public class StarfireEditor : ModuleRules
{
    public StarfireEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "UnrealEd",
                "Starfire"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "PropertyEditor", 
                "DetailCustomizations", 
                "EditorStyle"
            }
        );
    }
}