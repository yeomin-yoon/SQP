// Copyright Epic Games, Inc. All Rights Reserved.

#include "SQPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "HostSideLobbyMenuWidget.h"
#include "SQP.h"
#include "SQP_GM_Lobby.h"
#include "SQP_PS_Lobby.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASQPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Contexts
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}