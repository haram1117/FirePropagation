// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimulationWindowCommands.h"

#define LOCTEXT_NAMESPACE "FSimulationWindowModule"

void FSimulationWindowCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SimulationWindow", "Bring up SimulationWindow window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
