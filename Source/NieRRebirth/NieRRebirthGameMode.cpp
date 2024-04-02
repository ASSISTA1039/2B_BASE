// Copyright Epic Games, Inc. All Rights Reserved.

#include "NieRRebirthGameMode.h"
#include "NieRRebirthCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANieRRebirthGameMode::ANieRRebirthGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
