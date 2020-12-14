// Copyright Epic Games, Inc. All Rights Reserved.

#include "RacingGameGameMode.h"
#include "RacingGamePawn.h"
#include "RacingGameHud.h"

ARacingGameGameMode::ARacingGameGameMode()
{
	DefaultPawnClass = ARacingGamePawn::StaticClass();
	HUDClass = ARacingGameHud::StaticClass();
}
