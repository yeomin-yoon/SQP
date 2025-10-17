// Copyright Epic Games, Inc. All Rights Reserved.


#include "SQPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "HostSideLobbyMenuWidget.h"
#include "SQP.h"
#include "SQPGameState.h"
#include "SQPLobbyGameMode.h"
#include "SQPPlayerState.h"
#include "Blueprint/UserWidget.h"

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

void ASQPPlayerController::RequestSetReadyState(bool bNewReadyState)
{
	// 서버 RPC 호출
	Server_SetReadyState(bNewReadyState);
}





void ASQPPlayerController::LeaveLobby()
{
	// HasAuthority()는 현재 코드를 실행하는 인스턴스가 서버 권한을 가졌는지 확인합니다.
	// 리슨 서버에서는 호스트(서버)의 PlayerController만 true를 반환합니다.
	if (HasAuthority()) 
	{
		// --- 서버 (호스트)의 로직 ---
		// 호스트가 로비를 떠나면 세션이 종료됩니다.
		// ServerTravel은 서버에 연결된 모든 클라이언트를 지정된 맵으로 이동시킨 후,
		// 서버 자신도 해당 맵으로 이동합니다.
		if (UWorld* World = GetWorld())
		{
			// 더 이상 Listen 서버가 아니므로 '?listen' 옵션을 제거합니다.
			World->ServerTravel(TEXT("/Game/Maps/MainMenu")); 
		}
	}
	else 
	{
		// --- 클라이언트의 로직 ---
		// 클라이언트는 단순히 서버와의 연결을 끊고 자신의 메인 메뉴 맵으로 돌아갑니다.
		// ClientTravel은 이 PlayerController가 제어하는 클라이언트만 지정된 맵으로 이동시킵니다.
		ClientTravel(TEXT("/Game/Maps/MainMenu"), ETravelType::TRAVEL_Absolute);
	}
}

void ASQPPlayerController::Client_OnPostLogin_Implementation(const TArray<FPlayerInfo>& ExistingPlayerInfoArray)
{
	LobbyMenuWidget->OnPostLogin(ExistingPlayerInfoArray);
}

void ASQPPlayerController::Client_ReceiveEnterPlayerInfo_Implementation(FPlayerInfo NewPlayerInfo)
{
	LobbyMenuWidget->OnOtherPlayerEnter(NewPlayerInfo);		
}

void ASQPPlayerController::Client_ReceiveExitPlayerInfo_Implementation(FPlayerInfo OldPlayerInfo)
{
	LobbyMenuWidget->OnOtherPlayerExit(OldPlayerInfo);
}




void ASQPPlayerController::Client_CreateLobbyWidget_Implementation(const TSubclassOf<UUserWidget> WidgetToShow)
{
	PRINTLOGNET(TEXT("Client RPC CreateLobbyWidget Start!"));
	
	//뷰포트에 전달받은 위젯 클래스의 인스턴스를 생성하여 추가
	LobbyMenuWidget = Cast<ULobbyMenuWidgetBase>(CreateWidget(GetWorld(), WidgetToShow));
	LobbyMenuWidget->AddToViewport();
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	PRINTLOGNET(TEXT("Client RPC CreateLobbyWidget End!"));
}

void ASQPPlayerController::Client_UpdateReadyStatusInUI_Implementation(bool bIsReady)
{
	// 여기에 위젯(UI)의 모습을 변경하는 코드를 작성
	// 예: if(LobbyWidget) LobbyWidget->SetPlayerReadyIndicator(bIsReady);
}

void ASQPPlayerController::Server_SetReadyState_Implementation(const bool Value)
{
	if (HasAuthority())
	{
		//자신의 플레이어 스테이트에 접근하여 변경
		if (ASQPPlayerState* PS = GetPlayerState<ASQPPlayerState>())
		{
			PS->SetReadyState(Value);
		}	
	}
}