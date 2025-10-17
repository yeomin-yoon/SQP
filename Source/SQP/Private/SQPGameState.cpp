// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPGameState.h"
#include "LobbyMenuWidgetBase.h"
#include "SQPLobbyGameMode.h"
#include "SQPPaintWorldSubsystem.h"
#include "SQPPlayerController.h"
#include "SQPPlayerState.h"

class ASQPPlayerState;

void ASQPGameState::Multicast_PaintRenderTarget_Implementation(const FVector Start, const FVector End, const uint8 BrushIndex, const float BrushSize)
{
	if (const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>())
	{
		Subsystem->TryPaint(Start, End, BrushIndex, BrushSize);
	}
}

void ASQPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ASQPGameState::OnNewPlayerLogin(APlayerController* NewPlayer)
{
	//새로 들어온 친구들에게 미리 들어와있던 친구들의 데이터를 넘겨준다
	if (const auto NewPC = Cast<ASQPPlayerController>(NewPlayer))
	{
		NewPC->Client_OnPostLogin(ExistingPlayerInfoArray);
	}
	
	//이미 들어와있던 친구들에게 새로 들어온 친구들의 데이터를 넘겨준다
	if (const auto NewPS = NewPlayer->GetPlayerState<ASQPPlayerState>())
	{
		const auto NewPlayerInfo = NewPS->GetPlayerInfo();
		ExistingPlayerInfoArray.Add(NewPlayerInfo);

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (const auto OldPC = Cast<ASQPPlayerController>(It->Get()); OldPC && OldPC != NewPlayer)
			{
				OldPC->Client_ReceiveEnterPlayerInfo(NewPlayerInfo);
			}
		}
	}
}

void ASQPGameState::OnOldPlayerLogout(APlayerController* OldPlayer)
{
	
}

void ASQPGameState::OnRep_ExistingPlayerInfoArray()
{
	
}
