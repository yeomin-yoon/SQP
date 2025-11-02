// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_GM_Lobby.h"

#include "HostSideLobbyMenuWidget.h"
#include "SQP.h"
#include "SQP_GI.h"
#include "SQPGameState.h"
#include "SQPPlayerController.h"
#include "SQP_GS_Lobby.h"
#include "SQP_PC_Lobby.h"
#include "SQP_PS_LobbyRoomComponent.h"
#include "SQP_PS_Master.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/GameStateBase.h"

ASQP_GM_Lobby::ASQP_GM_Lobby()
{
	if (static ConstructorHelpers::FClassFinder<ASQPPlayerController>
		Finder(TEXT("/Game/Splatoon/Blueprint/LobbyLevel/BP_SQP_PC_Lobby.BP_SQP_PC_Lobby_C"));
		Finder.Succeeded())
	{
		PlayerControllerClass = Finder.Class;
	}

	if (static ConstructorHelpers::FClassFinder<ASQP_PS_Master>
		Finder(TEXT("/Game/Splatoon/Blueprint/Default/BP_SQP_PS_Master.BP_SQP_PS_Master_C"));
		Finder.Succeeded())
	{
		PlayerStateClass = Finder.Class;
	}

	if (static ConstructorHelpers::FClassFinder<ASQPGameState>
		Finder(TEXT("/Game/Splatoon/Blueprint/LobbyLevel/BP_SQP_GS_Lobby.BP_SQP_GS_Lobby_C"));
		Finder.Succeeded())
	{
		GameStateClass = Finder.Class;
	}

	bUseSeamlessTravel = true;
}

ASQP_PC_Lobby* ASQP_GM_Lobby::GetHostPlayerController() const
{
	return Cast<ASQP_PC_Lobby>(GetWorld()->GetFirstPlayerController());
}

void ASQP_GM_Lobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PRINTLOGNET(TEXT("Lobby PostLogin Start!"));

	//정상 상황이라면 이 캐스팅은 성공해야 한다
	const auto NewPC = Cast<ASQP_PC_Lobby>(NewPlayer);
	if (NewPC == nullptr)
	{
		return;
	}

	//클라이언트가 생성하도록 명령할 위젯 클래스 선택
	TSubclassOf<UUserWidget> TargetWidgetClassToShow;
	
	if (GetNetMode() != NM_DedicatedServer && NewPC->IsLocalController())
	{
		//리슨 서버 호스트
		TargetWidgetClassToShow = ServerSideLobbyMenuWidgetClass;
	}
	else
	{
		//외부 클라이언트
		TargetWidgetClassToShow = ClientSideLobbyMenuWidgetClass;
	}

	//클라이언트에 지정한 위젯을 생성해서 보여주도록 명령
	NewPC->Client_CreateLobbyWidget(TargetWidgetClassToShow);

	PRINTLOGNET(TEXT("Lobby PostLogin End!"));
}

void ASQP_GM_Lobby::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	//게임 스테이트에 나가는 플레이어의 정보 제거
	if (const auto GS = Cast<ASQP_GS_Lobby>(GetWorld()->GetGameState()))
	{
		GS->OnOldPlayerLogout(Exiting);
	}
}

bool ASQP_GM_Lobby::CheckAllPlayersReady() const
{
	//최소 시작 인원
	constexpr int32 MinPlayersToStart = 2;

	//최소 인원 미달
	if (GetWorld()->GetGameState()->PlayerArray.Num() < MinPlayersToStart)
	{
		PRINTLOGNET(TEXT("There is no Enough Player!"));
		return false;
	}

	int32 ReadyCount = 0;
	
	//게임 스테이트의 플레이어 목록을 순회
	for (APlayerState* PS : GetWorld()->GetGameState()->PlayerArray)
	{
		//서버의 플레이어 컨트롤러라면 건너뛴다
		if (PS->GetPlayerController() == GetWorld()->GetFirstPlayerController())
		{
			ReadyCount++;
			continue;
		}
		
		//한 명이라도 준비가 안됐다면 게임을 시작할 준비가 되지 않은 것이다
		if (const auto PSMaster = Cast<ASQP_PS_Master>(PS))
		{
			if (PSMaster->LobbyRoom->LOBBY_STATE == ELobbyState::UnreadyClient)
			{
				PRINTLOGNET(TEXT("There is Unready Player!"));
				return false;	
			}

			ReadyCount++;
		}
	}

	PRINTLOGNET(TEXT("There is %d / %d Ready in the Lobby!"), ReadyCount, GetWorld()->GetGameState()->PlayerArray.Num());

	return ReadyCount >= GetWorld()->GetGameState()->PlayerArray.Num();
}

void ASQP_GM_Lobby::MoveToGameMap()
{
	//이동 가능 상태
	if (CheckAllPlayersReady())
	{
		if (const auto GI = Cast<USQP_GI>(GetWorld()->GetGameInstance()))
		{
			//적절한 페인트 룸 레벨을 선정한다
			FString TagetLevel;
			
			if (GI->bPlayground)
			{
				TagetLevel = TEXT("CatchMind");
			}
			else
			{
				TagetLevel = GI->GetTargetPaintRoomSave().Level.Equals("") ? TEXT("ArtGallery") : GI->GetTargetPaintRoomSave().Level;
			}

			TagetLevel += TEXT("?seamless");
			
			//클라이언트와 함께 페인트 룸으로 이동한다
			GetWorld()->ServerTravel(TagetLevel);	
		}
	}
}

void ASQP_GM_Lobby::KickPlayerByUniqueId(const FString& PlayerUniqueId)
{
	const FText KickReason = FText::FromString(TEXT("호스트에 의해 로비에서 추방되었습니다."));
	
	//모든 플레이어 컨트롤러를 순회하여 ID가 일치하는 플레이어 검색
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (const APlayerController* TargetPC = It->Get(); TargetPC && TargetPC->PlayerState)
		{
			//PlayerState에서 고유 ID를 가져와 비교
			if (TargetPC->PlayerState->GetUniqueId()->ToString() == PlayerUniqueId)
			{
				if (const UNetConnection* Connection = Cast<UNetConnection>(TargetPC->Player))
				{
					GameSession->KickPlayer(Connection->PlayerController, KickReason);
				}
                
				return; 
			}
		}
	}
}