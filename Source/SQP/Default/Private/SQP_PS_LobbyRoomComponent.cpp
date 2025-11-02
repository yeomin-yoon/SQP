// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PS_LobbyRoomComponent.h"

#include "LobbyMenuWidgetBase.h"
#include "SQP.h"
#include "SQP_GI.h"
#include "SQP_GM_Lobby.h"
#include "SQP_GS_Lobby.h"
#include "SQP_PC_Lobby.h"
#include "SQP_PS_Master.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

USQP_PS_LobbyRoomComponent::USQP_PS_LobbyRoomComponent()
{
	LobbyState = ELobbyState::None;
}

void USQP_PS_LobbyRoomComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UGameplayStatics::GetCurrentLevelName(GetWorld()).Equals("Lobby") == false)
	{
		return;
	}

	if (GetOwner()->HasAuthority())
	{
		//자신의 상태를 확인하고 상태을 정한다
		if (GetBindingPSMaster()->GetPlayerController() == GetWorld()->GetFirstPlayerController())
		{
			LobbyState = ELobbyState::Host;
		}
		else
		{
			LobbyState = ELobbyState::UnreadyClient;
		}	
	}

	if (const auto GI = GetSQPGameInstance())
	{
		//서버에 자신의 유저 정보를 전송한다
		Server_NotifyUserInfo(GI->UserInfo);	
	}
}

void USQP_PS_LobbyRoomComponent::InitializeComponents()
{
	//월드 퍼스트 플레이어 컨트롤러와 이 플레이어 스테이트를 소유한 플레이어 컨트롤러가 일치하면 
	LobbyState = GetWorldFirstPCMaster() == GetBindingPSMaster()->GetPlayerController() ? ELobbyState::Host : ELobbyState::UnreadyClient;
}

void USQP_PS_LobbyRoomComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USQP_PS_LobbyRoomComponent, LobbyState);
}

void USQP_PS_LobbyRoomComponent::Server_NotifyUserInfo_Implementation(const FUserInfo UserInfo)
{
	Super::Server_NotifyUserInfo_Implementation(UserInfo);

	//게임 스테이트에 새로운 플레이어의 정보 추가
	if (const auto GS = Cast<ASQP_GS_Lobby>(GetWorld()->GetGameState()))
	{
		//서버 측의 PS가 업데이트됐으므로
		GetBindingPSMaster()->SetPlayerName(UserInfo.UserNickname);
		
		GS->OnNewPlayerLogin(GetBindingPSMaster()->GetPlayerController());

		PRINTLOGNET(TEXT("Lobby Notify User Info!"));
	}	
}

ELobbyState USQP_PS_LobbyRoomComponent::GetLobbyState() const
{
	return LobbyState;
}

void USQP_PS_LobbyRoomComponent::SetLobbyState(const ELobbyState Value)
{
	if (GetOwner()->HasAuthority())
	{
		LobbyState = Value;

		OnRep_LobbyState();
	}
}

FPlayerInfo USQP_PS_LobbyRoomComponent::GetPlayerInfo() const
{
	const ASQP_PS_Master* PSMaster = GetBindingPSMaster();
	const FString ID = PSMaster->GetUniqueId()->ToString();
	const FString Name = PSMaster->GetPlayerName();
	return FPlayerInfo(ID, Name, LobbyState);
}

void USQP_PS_LobbyRoomComponent::OnRep_LobbyState() const
{
	PRINTLOGNET(TEXT("OnRep_LobbyState"));
	
	//리플리케이션된 타 플레이어 컨트롤러의 정보를 바탕으로, 현 시스템의 로비 위젯을 업데이트
	const FString PlayerUniqueId = GetBindingPSMaster()->GetUniqueId()->ToString();
	GetWorldFirstPCMaster()->LobbyMenuWidget->UpdatePlayerLobbyState(PlayerUniqueId, LobbyState);	
}