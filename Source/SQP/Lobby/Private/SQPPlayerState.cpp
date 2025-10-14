// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPPlayerState.h"
#include "SQPPlayerController.h"
#include "Net/UnrealNetwork.h"

ASQPPlayerState::ASQPPlayerState()
{
	bIsReady = false;
}

void ASQPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// bIsReady 변수를 모든 클라이언트에게 복제하도록 등록
	DOREPLIFETIME(ASQPPlayerState, bIsReady);
}

//서버에서 호출되어 상태를 변경
void ASQPPlayerState::SetReadyState(bool bReady)
{
	if (HasAuthority()) // 서버인지 확인
	{
		bIsReady = bReady;
		
		// 서버에서는 OnRep 함수가 자동 호출되지 않으므로 직접 호출
		OnRep_bIsReady();
	}
}

//클라이언트에서 bIsReady 값이 변경되면 자동 호출
void ASQPPlayerState::OnRep_bIsReady()
{
	// 소유자 PlayerController를 찾아 UI 업데이트 함수 호출
	if (ASQPPlayerController* PC = Cast<ASQPPlayerController>(GetOwner()))
	{
		PC->Client_UpdateReadyStatusInUI(bIsReady); // UI 업데이트 함수
	}
}