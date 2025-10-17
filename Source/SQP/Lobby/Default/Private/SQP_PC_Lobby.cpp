// Fill out your copyright notice in the Description page of Project Settings.


#include "SQP_PC_Lobby.h"
#include "SQP.h"
#include "SQP_GM_Lobby.h"
#include "SQP_PS_Lobby.h"
#include "LobbyMenuWidgetBase.h"
#include "Kismet/GameplayStatics.h"


void ASQP_PC_Lobby::LeaveLobby()
{
	//로비 메뉴 위젯을 제거
	LobbyMenuWidget->RemoveFromParent();
	
	if (HasAuthority()) 
	{
		//--- 호스트 ---
		//ServerTravel은 서버에 연결된 모든 클라이언트를 지정된 맵으로 이동시킨 후 자신도 이동한다
		if (UWorld* World = GetWorld())
		{
			//메인 레벨에서는 리슨 서버가 아니므로
			World->ServerTravel(TEXT("Main")); 
		}
	}
	else 
	{
		//--- 클라이언트 ---
		//ClientTravel은 이 플레이어 컨트롤러가 제어하는 클라이언트만 지정된 맵으로 이동시킨다
		ClientTravel(TEXT("Main"), ETravelType::TRAVEL_Absolute);
	}
}

void ASQP_PC_Lobby::ClientWasKicked_Implementation(const FText& KickReason)
{
	PRINTLOGNET(TEXT("%s"), *KickReason.ToString());

	UGameplayStatics::OpenLevel(this, FName("Main"));
	
	Super::ClientWasKicked_Implementation(KickReason);

	PRINTLOGNET(TEXT("%s"), *KickReason.ToString());
}

void ASQP_PC_Lobby::Client_OnPostLogin_Implementation(const TArray<FPlayerInfo>& ExistingPlayerInfoArray)
{
	LobbyMenuWidget->OnPostLogin(ExistingPlayerInfoArray);
}

void ASQP_PC_Lobby::Client_ReceiveEnterPlayerInfo_Implementation(FPlayerInfo NewPlayerInfo)
{
	LobbyMenuWidget->OnOtherPlayerEnter(NewPlayerInfo);		
}

void ASQP_PC_Lobby::Client_ReceiveExitPlayerInfo_Implementation(FPlayerInfo OldPlayerInfo)
{
	LobbyMenuWidget->OnOtherPlayerExit(OldPlayerInfo);
}




void ASQP_PC_Lobby::Client_CreateLobbyWidget_Implementation(const TSubclassOf<UUserWidget> WidgetToShow)
{
	PRINTLOGNET(TEXT("Client RPC CreateLobbyWidget Start!"));
	
	//뷰포트에 전달받은 위젯 클래스의 인스턴스를 생성하여 추가
	LobbyMenuWidget = Cast<ULobbyMenuWidgetBase>(CreateWidget(GetWorld(), WidgetToShow));
	LobbyMenuWidget->AddToViewport();
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	PRINTLOGNET(TEXT("Client RPC CreateLobbyWidget End!"));
}

void ASQP_PC_Lobby::Server_SetReadyState_Implementation(const bool Value)
{
	//요청대로 플레이어 스테이트 변경, 이후 자동으로 모든 클라이언트에 리플리케이션
	Cast<ASQP_PS_Lobby>(PlayerState)->READY_STATE = Value;
	
	// Multicast_UpdateReadyState(this, Value);
}

void ASQP_PC_Lobby::Multicast_UpdateReadyState_Implementation(APlayerController* ReadyPlayer, const bool Value)
{
	// //자신 이외의 플레이어의 변경 상태만 업데이트 하면 된다
	// if (ReadyPlayer != this)
	// {
	// 	const FString PlayerUniqueId = ReadyPlayer->PlayerState->GetUniqueId()->ToString();
	// 	LobbyMenuWidget->UpdatePlayerInfoReady(PlayerUniqueId, Value);	
	// }
}