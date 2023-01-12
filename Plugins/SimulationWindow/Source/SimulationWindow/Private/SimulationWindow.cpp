// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimulationWindow.h"
#include "SimulationWindowStyle.h"
#include "SimulationWindowCommands.h"
#include "LevelEditor.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"

static const FName SimulationWindowTabName("SimulationWindow");

#define LOCTEXT_NAMESPACE "FSimulationWindowModule"

void FSimulationWindowModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimulationWindowStyle::Initialize();
	FSimulationWindowStyle::ReloadTextures();

	FSimulationWindowCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSimulationWindowCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSimulationWindowModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSimulationWindowModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SimulationWindowTabName, FOnSpawnTab::CreateRaw(this, &FSimulationWindowModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSimulationWindowTabTitle", "SimulationWindow"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSimulationWindowModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSimulationWindowStyle::Shutdown();

	FSimulationWindowCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SimulationWindowTabName);
}

void FSimulationWindowModule::MeshUpdate(const FAssetData& InAssetData)
{
	const UStaticMesh* const MeshAsset = Cast<const UStaticMesh>(InAssetData.GetAsset());
	// StaticMesh = MeshAsset;
	
	if(GEngine && MeshAsset)
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Black, TEXT("HIHI"));
}

TSharedRef<SDockTab> FSimulationWindowModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(1));
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot().HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Combustion Simulation Build Window")))
			]
			+SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot().HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Target Mesh")))
				]
				+SHorizontalBox::Slot().HAlign(HAlign_Center)
				[
					SAssignNew(MeshBox, SObjectPropertyEntryBox)
					.ThumbnailPool(AssetThumbnailPool)
					.DisplayThumbnail(true)
					.AllowedClass(UStaticMesh::StaticClass())
					.DisplayBrowse(true)
					.OnObjectChanged_Raw(this, &FSimulationWindowModule::MeshUpdate)
					.EnableContentPicker(true)
					.DisplayUseSelected(true)
					.DisplayCompactSize(true)
				]
			]
			+SVerticalBox::Slot().HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Property")))
			]
			// Put your tab content here!
		];
}

void FSimulationWindowModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SimulationWindowTabName);
}

void FSimulationWindowModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSimulationWindowCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSimulationWindowCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimulationWindowModule, SimulationWindow)