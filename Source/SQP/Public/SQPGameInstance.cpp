// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"
#include "SQP.h"

void USQPGameInstance::HostGame()
{
	if (SessionInterface.IsValid())
	{
		if (SessionInterface->GetNamedSession(NAME_GameSession) != nullptr)
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}

		//세션 생성 완료 시 호출할 함수를 델리게이트에 바인딩
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USQPGameInstance::OnCreateSessionComplete);

		//세션 설정
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;				//LAN 매치 여부
		SessionSettings.NumPublicConnections = 4;		//최대 접속 인원 
		SessionSettings.bShouldAdvertise = true;		//세션 검색 가능
		SessionSettings.bUsesPresence = true;			//Presence 기능 사용
		SessionSettings.bAllowJoinInProgress = true;	//진행 중 참가 가능

		//세션 생성 요청
		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);

		PRINTINFO();
	}
}

void USQPGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//세션 생성 델리게이트 해제
	SessionInterface->ClearOnCreateSessionCompleteDelegates(this);

	PRINTINFO();
	
	if (bWasSuccessful)
	{
		//세션 생성이 성공하면 로비 맵으로 리슨 서버로 이동
		GetWorld()->ServerTravel("/Game/Maps/LobbyMap?listen");
		
	}
}

void USQPGameInstance::Init()
{
	Super::Init();

	//기본 온라인 서브시스템 가져오기
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		SessionInterface = Subsystem->GetSessionInterface();

		PRINTINFO();
	}
}

void USQPGameInstance::FindAndJoinGame()
{
    if (SessionInterface.IsValid())
    {
        // 1. 세션 검색 객체 생성
        SessionSearch = MakeShareable(new FOnlineSessionSearch());
        
        // 2. 검색 조건 설정
        SessionSearch->bIsLanQuery = true; // LAN 환경에서 검색
        SessionSearch->MaxSearchResults = 10; // 최대 검색 결과 수
        // Presence를 사용하는 세션을 찾도록 설정 (서버 HostGame 함수와 일치)
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

        UE_LOG(LogTemp, Warning, TEXT("Finding Sessions..."));

        // 3. 세션 검색 완료 델리게이트 바인딩
        SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USQPGameInstance::OnFindSessionsComplete);
        
        // 4. 세션 검색 시작
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}

void USQPGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
    // 검색 델리게이트 해제
    SessionInterface->ClearOnFindSessionsCompleteDelegates(this);

    if (bWasSuccessful && SessionSearch.IsValid() && SessionSearch->SearchResults.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sessions Found!"));

        // 5. 검색된 첫 번째 세션에 참가 시도
        const FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[0];
        
        // 6. 참가 완료 델리게이트 바인딩
        SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USQPGameInstance::OnJoinSessionComplete);
        
        // 7. 세션 참가 요청
        SessionInterface->JoinSession(0, NAME_GameSession, SearchResult);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not find any sessions."));
    }
}

void USQPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    // 참가 델리게이트 해제
    SessionInterface->ClearOnJoinSessionCompleteDelegates(this);

    if (Result == EOnJoinSessionCompleteResult::Success)
    {
        // 8. 세션 참가 성공 시 서버 주소(맵)로 이동
        APlayerController* PC = GetFirstLocalPlayerController();
        if (PC)
        {
            FString ConnectInfo;
            // 서버 주소를 가져와서 ClientTravel 실행
            if (SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
            {
                UE_LOG(LogTemp, Warning, TEXT("Joining Server at: %s"), *ConnectInfo);
                PC->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to join session."));
    }
}