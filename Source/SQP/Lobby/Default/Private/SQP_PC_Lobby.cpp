// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PC_Lobby.h"

#include "ActiveButton.h"
#include "ActiveButtonBox.h"
#include "HostSideLobbyMenuWidget.h"
#include "SQP.h"
#include "SQP_GM_Lobby.h"
#include "SQP_PS_Lobby.h"
#include "LobbyMenuWidgetBase.h"
#include "SQPGameInstance.h"
#include "SQP_GS_Lobby.h"
#include "Kismet/GameplayStatics.h"

ASQP_GM_Lobby* ASQP_PC_Lobby::GetHostGameMode() const
{
	return Cast<ASQP_GM_Lobby>(GetWorld()->GetAuthGameMode());	
}

void ASQP_PC_Lobby::LeaveLobby() const
{
	//로비 메뉴 위젯을 제거
	LobbyMenuWidget->RemoveFromParent();
	
	//세션을 파괴하고 메인 메뉴로 복귀한다
	if (const auto GI = Cast<USQPGameInstance>(GetGameInstance()))
	{
		GI->TerminateMySession();
	}
}

void ASQP_PC_Lobby::ClientWasKicked_Implementation(const FText& KickReason)
{
	PRINTLOGNET(TEXT("%s"), *KickReason.ToString());

	UGameplayStatics::OpenLevel(this, FName("Main"));
	
	Super::ClientWasKicked_Implementation(KickReason);
}

void ASQP_PC_Lobby::Client_OnPostLogin_Implementation(const TArray<FPlayerInfo>& ExistingPlayerInfoArray)
{
	if (LobbyMenuWidget)
	{
		LobbyMenuWidget->OnPostLogin(ExistingPlayerInfoArray);
	}
}

void ASQP_PC_Lobby::Client_ReceiveEnterPlayerInfo_Implementation(FPlayerInfo NewPlayerInfo)
{
	if (LobbyMenuWidget)
	{
		LobbyMenuWidget->OnOtherPlayerEnter(NewPlayerInfo);			
	}
}

void ASQP_PC_Lobby::Client_ReceiveExitPlayerInfo_Implementation(FPlayerInfo OldPlayerInfo)
{
	if (LobbyMenuWidget)
	{
		LobbyMenuWidget->OnOtherPlayerExit(OldPlayerInfo);	
	}
}

void ASQP_PC_Lobby::Client_CreateLobbyWidget_Implementation(const TSubclassOf<UUserWidget> TargetWidgetClass)
{
	PRINTLOGNET(TEXT("Client RPC CreateLobbyWidget Start!"));
	
	//뷰포트에 전달받은 위젯 클래스의 인스턴스를 생성하여 추가
	LobbyMenuWidget = Cast<ULobbyMenuWidgetBase>(CreateWidget(GetWorld(), TargetWidgetClass));
	LobbyMenuWidget->AddToViewport();
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	PRINTLOGNET(TEXT("Client RPC CreateLobbyWidget End!"));
}

void ASQP_PC_Lobby::Server_SetLobbyState_Implementation(const ELobbyState Value)
{
	//현존 플레이어 정보를 업데이트 한다
	for (auto PlayerInfo : Cast<ASQP_GS_Lobby>(GetWorld()->GetGameState())->ExistingPlayerInfoArray)
	{
		if (PlayerInfo.PlayerUniqueId == PlayerState->GetUniqueId()->ToString())
		{
			PlayerInfo.LobbyState = Value;
		}
	}
	
	//요청대로 플레이어 스테이트 변경, 이후 자동으로 모든 클라이언트에 리플리케이션
	Cast<ASQP_PS_Lobby>(PlayerState)->LOBBY_STATE = Value;
	
	//전체 플레이어의 준비 상태에 따라서 시작 버튼을 활성화
	if (const auto HostLobbyMenuWidget = Cast<UHostSideLobbyMenuWidget>(GetHostGameMode()->GetHostPlayerController()->LobbyMenuWidget))
	{
		const bool bIsAllReady = GetHostGameMode()->CheckAllPlayersReady();
		HostLobbyMenuWidget->StartButton->SetActive(bIsAllReady);
	}
}