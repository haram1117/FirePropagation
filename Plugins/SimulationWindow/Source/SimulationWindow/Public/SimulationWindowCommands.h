// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SimulationWindowStyle.h"

class FSimulationWindowCommands : public TCommands<FSimulationWindowCommands>
{
public:

	FSimulationWindowCommands()
		: TCommands<FSimulationWindowCommands>(TEXT("SimulationWindow"), NSLOCTEXT("Contexts", "SimulationWindow", "SimulationWindow Plugin"), NAME_None, FSimulationWindowStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};