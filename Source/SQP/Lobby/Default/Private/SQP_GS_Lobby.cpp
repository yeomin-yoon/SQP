// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_GS_Lobby.h"
#include "SQP_GM_Lobby.h"
#include "SQP_PC_Lobby.h"
#include "SQP_PS_Lobby.h"
#include "Net/UnrealNetwork.h"

void ASQP_GS_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASQP_GS_Lobby, ExistingPlayerInfoArray);
}

void ASQP_GS_Lobby::OnNewPlayerLogin(APlayerController* LoginPlayer)
{
	//새로 들어온 친구들에게 미리 들어와있던 친구들의 데이터를 넘겨준다
	if (const auto NewPC = Cast<ASQP_PC_Lobby>(LoginPlayer))
	{
		NewPC->Client_OnPostLogin(ExistingPlayerInfoArray);
	}
	
	//이미 들어와있던 친구들에게 새로 들어온 친구들의 데이터를 넘겨준다
	if (const auto NewPS = LoginPlayer->GetPlayerState<ASQP_PS_Lobby>())
	{
		const auto NewPlayerInfo = NewPS->GetPlayerInfo();
		ExistingPlayerInfoArray.Add(NewPlayerInfo);

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (const auto OldPC = Cast<ASQP_PC_Lobby>(It->Get()); OldPC && OldPC != LoginPlayer)
			{
				OldPC->Client_ReceiveEnterPlayerInfo(NewPlayerInfo);
			}
		}
	}
}

void ASQP_GS_Lobby::OnOldPlayerLogout(const AController* LogoutPlayer)
{
	//이미 들어와있던 친구들에게 새로 들어온 친구들의 데이터를 넘겨준다
	if (const auto OutPC = LogoutPlayer->GetPlayerState<ASQP_PS_Lobby>())
	{
		const auto OutPlayerInfo = OutPC->GetPlayerInfo();
		ExistingPlayerInfoArray.Remove(OutPlayerInfo);

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (const auto OldPC = Cast<ASQP_PC_Lobby>(It->Get()); OldPC && OldPC != LogoutPlayer)
			{
				OldPC->Client_ReceiveExitPlayerInfo(OutPlayerInfo);
			}
		}
	}
}

void ASQP_GS_Lobby::OnRep_ExistingPlayerInfoArray()
{
	
}