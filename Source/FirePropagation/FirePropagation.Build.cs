// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FirePropagation : ModuleRules
{
	public FirePropagation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RenderCore", "RawMesh", "Slate", "SlateCore", "UnrealEd", "PropertyEditor"
			, "EditorScriptingUtilities", 
			"EditorStyle",
			"DesktopPlatform"});

		PrivateDependencyModuleNames.AddRange(new string[] { "MeshDescription", "StaticMeshDescription", "DynamicMesh", "ModelingComponents", "MeshConversion", "MeshPaint"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// PublicIncludePaths.AddRange(new string[] { "FirePropagation/Public" });
		// PrivateIncludePaths.AddRange(new string[] { "FirePropagation/Private" });
		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
