// Copyright Epic Games, Inc. All Rights Reserved.


#include "SQPPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "SQPPlayerState.h"

class ASQPPlayerState;

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

void ASQPPlayerController::Client_CreateLobbyUI_Implementation()
{
	
}

// PlayerState의 OnRep 함수에서 호출되어 실제 UI를 업데이트하는 함수
void ASQPPlayerController::Client_UpdateReadyStatusInUI_Implementation(bool bIsReady)
{
	// 여기에 위젯(UI)의 모습을 변경하는 코드를 작성
	// 예: if(LobbyWidget) LobbyWidget->SetPlayerReadyIndicator(bIsReady);
}

// RPC 유효성 검사 함수
bool ASQPPlayerController::Server_SetReadyState_Validate(bool bNewReadyState)
{
	return true; // 간단한 유효성 검사
}

// 서버에서 실제 실행되는 함수
void ASQPPlayerController::Server_SetReadyState_Implementation(bool bNewReadyState)
{
	// 자신의 PlayerState를 가져와 상태 변경
	if (ASQPPlayerState* PS = GetPlayerState<ASQPPlayerState>())
	{
		PS->SetReadyState(bNewReadyState);
	}
}