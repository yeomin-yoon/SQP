// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPGameState.h"
#include "LobbyMenuWidgetBase.h"
#include "SQP_GM_Lobby.h"
#include "SQPPaintWorldSubsystem.h"
#include "SQPPlayerController.h"
#include "SQP_PC_Lobby.h"
#include "SQP_PS_Lobby.h"

class ASQP_PS_Lobby;

void ASQPGameState::Multicast_PaintRenderTarget_Implementation(const FVector Start, const FVector End, const uint8 BrushIndex, const float BrushSize)
{
	if (const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>())
	{
		Subsystem->TryPaint(Start, End, BrushIndex, BrushSize);
	}
}