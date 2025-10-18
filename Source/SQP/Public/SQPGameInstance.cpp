// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPGameInstance.h"
#include "SQP.h"

void USQPGameInstance::Init()
{
	Super::Init();

	//초기 게임 상태
	GAME_STATE = EGameState::Main;

	//네트워크 실패 콜백 등록
	GEngine->OnNetworkFailure().AddUObject(this, &USQPGameInstance::OnNetworkFailure);
}

void USQPGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	//맵 경로
	FString TargetMapPath = "";

	// 현재 게임 상태에 따라 이동할 맵을 결정
	switch (CurrentGameState)
	{
	case EGameState::Lobby:
		{
			//로비에 있다가 튕겼다면
			PRINTLOG(TEXT("Network Failure in Lobby. Returning to Host Browser."));
			TargetMapPath = TEXT("Main");
			break;
		}
	case EGameState::InGame:
		{
			//게임 플레이 중에 튕겼다면
			PRINTLOG(TEXT("Network Failure in Game. Returning to Main Menu."));
			TargetMapPath = TEXT("Main");
			break;
		}
	default:
		{
			//예기치 못한 네트워크 실패
			PRINTLOG(TEXT("Unexpected Network Failure. Returning to Main Menu."));
			TargetMapPath = TEXT("Main");
			break;
		}
	}

	//결정된 맵으로 클라이언트와 함께 이동
	GetWorld()->ServerTravel(TargetMapPath, true);
}