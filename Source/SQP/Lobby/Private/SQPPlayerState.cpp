// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPPlayerState.h"

#include "SQPLobbyGameMode.h"
#include "SQPPlayerController.h"
#include "Net/UnrealNetwork.h"

ASQPPlayerState::ASQPPlayerState()
{
	bIsReady = false;
}

void ASQPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASQPPlayerState, bIsReady);
}

void ASQPPlayerState::SetReadyState(const bool Value)
{
	if (HasAuthority())
	{
		bIsReady = Value;
		
		//수동 호출
		OnRep_bIsReady();
	}	
}

FPlayerInfo ASQPPlayerState::GetPlayerInfo()
{
	FString Id = TEXT("Invalid");
	if (const auto UniquePtr = GetUniqueId(); UniquePtr->IsValid())
	{
		Id = UniquePtr->ToString();
	}

	return FPlayerInfo(Id, GetPlayerName());
}

void ASQPPlayerState::OnRep_bIsReady()
{
	// 소유자 PlayerController를 찾아 UI 업데이트 함수 호출
	if (ASQPPlayerController* PC = Cast<ASQPPlayerController>(GetOwner()))
	{
		PC->Client_UpdateReadyStatusInUI(bIsReady); // UI 업데이트 함수
	}
}