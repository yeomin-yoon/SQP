// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PS_Lobby.h"

#include "LobbyMenuWidgetBase.h"
#include "SQP.h"
#include "SQP_GM_Lobby.h"
#include "SQP_PC_Lobby.h"
#include "Net/UnrealNetwork.h"

ASQP_PS_Lobby::ASQP_PS_Lobby()
{
	LobbyState = ELobbyState::None;
}

void ASQP_PS_Lobby::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//월드 퍼스트 플레이어 컨트롤러와 이 플레이어 스테이트를 소유한 플레이어 컨트롤러가 일치하면 
	LobbyState = GetLobbyPC() == GetPlayerController() ? ELobbyState::Host : ELobbyState::UnreadyClient;
}

void ASQP_PS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASQP_PS_Lobby, LobbyState);
}

FPlayerInfo ASQP_PS_Lobby::GetPlayerInfo()
{
	return FPlayerInfo(GetUniqueId()->ToString(), GetPlayerName(), LobbyState);
}

ASQP_PC_Lobby* ASQP_PS_Lobby::GetLobbyPC() const
{
	return Cast<ASQP_PC_Lobby>(GetWorld()->GetFirstPlayerController());
}

void ASQP_PS_Lobby::OnRep_LobbyState()
{
	PRINTLOGNET(TEXT("OnRep_LobbyState"));
	
	//리플리케이션된 타 플레이어 컨트롤러의 정보를 바탕으로, 현 시스템의 로비 위젯을 업데이트
	const FString PlayerUniqueId = this->GetUniqueId()->ToString();
	GetLobbyPC()->LobbyMenuWidget->UpdatePlayerLobbyState(PlayerUniqueId, LobbyState);	
}