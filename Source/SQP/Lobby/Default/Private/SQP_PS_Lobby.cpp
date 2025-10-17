// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PS_Lobby.h"

#include "LobbyMenuWidgetBase.h"
#include "SQP_GM_Lobby.h"
#include "SQP_PC_Lobby.h"
#include "Net/UnrealNetwork.h"

ASQP_PS_Lobby::ASQP_PS_Lobby()
{
	bReadyState = false;
}

void ASQP_PS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASQP_PS_Lobby, bReadyState);
}

void ASQP_PS_Lobby::SetReadyState(const bool Value)
{
	//플레이어 스테이트를 업데이트 하는 작업은 서버만 할 수 있다
	if (HasAuthority())
	{
		//값 할당
		bReadyState = Value;
		
		//수동 호출
		OnRep_bReadyState();
	}	
}

FPlayerInfo ASQP_PS_Lobby::GetPlayerInfo()
{
	return FPlayerInfo(GetUniqueId()->ToString(), GetPlayerName());
}

void ASQP_PS_Lobby::OnRep_bReadyState()
{
	//리플리케이션된 플레이어 컨트롤러의 정보를 바탕으로 업데이트
	const FString PlayerUniqueId = this->GetUniqueId()->ToString();
	Cast<ASQP_PC_Lobby>(GetWorld()->GetFirstPlayerController())->LobbyMenuWidget->UpdatePlayerInfoReady(PlayerUniqueId, bReadyState);
}